from ultralytics import YOLO

if __name__ == "__main__":
    model = YOLO("yolov8n.pt")  # or "yolov8s.pt" etc.
    model.train(
        data="C:/Users/Hariz/Downloads/hackathon x entreprenuership/sketch_jun13b/dataset_yolo/data.yaml",
        epochs=50,
        imgsz=416,
        batch=8,
        workers=0  # optional: set to 0 to avoid multiprocessing issues
    )
