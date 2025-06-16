from ultralytics import YOLO

if __name__ == "__main__":
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))
    DATA_PATH = os.path.join(BASE_DIR, "data.yaml")
    model = YOLO("yolov8n.pt")  # or "yolov8s.pt" etc.
    model.train(
        data=DATA_PATH,
        epochs=50,
        imgsz=416,
        batch=8,
        workers=0  # optional: set to 0 to avoid multiprocessing issues
    )
