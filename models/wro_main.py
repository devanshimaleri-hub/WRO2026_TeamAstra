import cv2
import numpy as np
from picamera2 import Picamera2
import time
import serial


ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
time.sleep(2)

consider_object = 28000  # px

# -------- Function --------

def detect(mask, colour_name, box_colour):

    contours, _ = cv2.findContours(
        mask,
        cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE
    )

    if len(contours) == 0:
        return None

    biggest = max(contours, key=cv2.contourArea)

    area = cv2.contourArea(biggest)

    if area < MIN_AREA:
        return None

    x, y, w, h = cv2.boundingRect(biggest)

    return {
        "contour": biggest,
        "area": area,
        "x": x,
        "y": y,
        "w": w,
        "h": h,
        "name": colour_name,
        "color": box_colour
    }


# ---------------- Camera ----------------

picam2 = Picamera2()


config = picam2.create_preview_configuration(
    main={
        "size": (640, 480),
        "format": "RGB888"
    }
)

picam2.configure(config)
picam2.start()

time.sleep(2)


FRAME_WIDTH = 640
FRAME_HEIGHT = 480

CENTER_X = FRAME_WIDTH // 2
CENTER_Y = FRAME_HEIGHT // 2


# Divide screen into 4 equal vertical parts

right_line = FRAME_WIDTH // 4
middle_line = FRAME_WIDTH // 2
left_line = (FRAME_WIDTH * 3) // 4

MIN_AREA = 1000


# ---------- HSV Ranges ----------

lower_red1 = np.array([0, 160, 120])
upper_red1 = np.array([10, 255, 255])

lower_red2 = np.array([170, 160, 120])
upper_red2 = np.array([180, 255, 255])

lower_green = np.array([35, 120, 80])
upper_green = np.array([85, 255, 255])


# ---------- Morphology Kernel ----------

kernel = np.ones((5, 5), np.uint8)


# ---------------- Loop ----------------

while True:

    # Capture image from Raspberry Pi camera
    frame = picam2.capture_array()

    # Picamera2 gives RGB image
    # OpenCV expects BGR
    # frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)

    # Flip camera image horizontally
    frame = cv2.flip(frame, 1)
    # Flip camera image vertically
    frame = cv2.flip(frame, -1)

    # ---------- Draw vertical lines ----------

    cv2.line(
        frame,
        (right_line, 0),
        (right_line, FRAME_HEIGHT),
        (255, 255, 255),
        1
    )

    cv2.line(
        frame,
        (middle_line, 0),
        (middle_line, FRAME_HEIGHT),
        (255, 255, 255),
        1
    )

    cv2.line(
        frame,
        (left_line, 0),
        (left_line, FRAME_HEIGHT),
        (255, 255, 255),
        1
    )


    # ---------- Convert to HSV ----------

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)


    # ---------- RED ----------

    mask_red1 = cv2.inRange(
        hsv,
        lower_red1,
        upper_red1
    )

    mask_red2 = cv2.inRange(
        hsv,
        lower_red2,
        upper_red2
    )

    red_mask = mask_red1 | mask_red2


    # ---------- GREEN ----------

    green_mask = cv2.inRange(
        hsv,
        lower_green,
        upper_green
    )


    # ---------- Morphology ----------

    red_mask = cv2.morphologyEx(
        red_mask,
        cv2.MORPH_OPEN,
        kernel
    )

    red_mask = cv2.morphologyEx(
        red_mask,
        cv2.MORPH_CLOSE,
        kernel
    )


    green_mask = cv2.morphologyEx(
        green_mask,
        cv2.MORPH_OPEN,
        kernel
    )

    green_mask = cv2.morphologyEx(
        green_mask,
        cv2.MORPH_CLOSE,
        kernel
    )


    # ---------- Detect Objects ----------

    red = detect(
        red_mask,
        "RED",
        (0, 0, 255)
    )

    green = detect(
        green_mask,
        "GREEN",
        (0, 255, 0)
    )


    # ---------- Choose Closest Object ----------
    # Largest area = closest object

    target = None

    if red and green:

        target = (
            red
            if red["area"] > green["area"]
            else green
        )

    elif red:

        target = red

    elif green:

        target = green


    # ---------- Camera Center ----------

    cv2.circle(
        frame,
        (CENTER_X, CENTER_Y),
        6,
        (255, 255, 255),
        -1
    )


    # ==================================================
    # TARGET PROCESSING
    # ==================================================

    if target:

        x = target["x"]
        y = target["y"]
        w = target["w"]
        h = target["h"]

        cx = x + w // 2
        cy = y + h // 2


        # ---------- Bounding Box ----------

        cv2.rectangle(
            frame,
            (x, y),
            (x + w, y + h),
            target["color"],
            2
        )


        # ---------- Left Edge ----------

        cv2.line(
            frame,
            (x, 0),
            (x, FRAME_HEIGHT),
            target["color"],
            2
        )


        # ---------- Right Edge ----------

        cv2.line(
            frame,
            (x + w, 0),
            (x + w, FRAME_HEIGHT),
            target["color"],
            2
        )


        # ---------- Object Center ----------

        cv2.circle(
            frame,
            (cx, cy),
            6,
            target["color"],
            -1
        )


        # ---------- Object Name ----------

        cv2.putText(
            frame,
            target["name"],
            (x, y - 10),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.6,
            target["color"],
            2
        )


        # ---------- Area ----------

        cv2.putText(
            frame,
            f"Area: {int(target['area'])} px^2",
            (x, y + h + 25),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.6,
            target["color"],
            2
        )


        # ==================================================
        # DECISION
        # ==================================================

        TOLERANCE = 10

        if target["name"] == "RED":

            # Left edge of red box
            edge = x

            if abs(edge - left_line) <= TOLERANCE:

                command = "f"

            elif edge < left_line:

                command = "r"

            else:

                command = "l"


        else:

            # Right edge of green box
            edge = x + w

            if abs(edge - right_line) <= TOLERANCE:

                command = "f"

            elif edge > right_line:

                command = "l"

            else:

                command = "r"



        if target["area"] >= consider_object:

            if target["name"] == "RED":

                print("hr")
                ser.write(b'x')

            else:

                print("hl")
                ser.write(b'z')

        else:

            if command == 'f':
                ser.write(b'f')
            if command == 'l':
                ser.write(b'l')
            if command == 'r':
                ser.write(b'r')

            print(command)


        # ==================================================
        # DYNAMIC SPEED
        # ==================================================

        if target["area"] < 10000:

            print("s255")

        elif target["area"] < 20000:

            print("s170")

        elif target["area"] < consider_object:

            print("s100")


        # ---------- Display Command ----------

        cv2.putText(
            frame,
            f"Command: {command}",
            (10, 35),
            cv2.FONT_HERSHEY_SIMPLEX,
            1,
            (0, 255, 255),
            2
        )


    # ---------- Display ----------

    cv2.imshow(
        "Detection",
        frame
    )


    # ---------- Quit ----------

    if cv2.waitKey(1) & 0xFF == ord('q'):

        break


# ---------------- Cleanup ----------------

picam2.stop()

cv2.destroyAllWindows()