import serial
import time
from PIL import Image

def send_image_to_display_batch(image_path, port='/dev/ttyACM0', baudrate=115200, max_width=320, max_height=240):
    """
    Отправляет изображение на дисплей пакетами для повышения скорости
    """
    ser = None
    
    try:
        # Открываем и изменяем размер изображения
        print(f"Opening image: {image_path}")
        image = Image.open(image_path)
        
        width, height = image.size
        print(f"Original size: {width}x{height}")
        
        # Уменьшаем размер
        if width > max_width or height > max_height:
            ratio = min(max_width/width, max_height/height)
            new_width = int(width * ratio)
            new_height = int(height * ratio)
            print(f"Resizing to: {new_width}x{new_height}")
            image = image.resize((new_width, new_height), Image.Resampling.LANCZOS)
            width, height = image.size
        
        print(f"Final size: {width}x{height} ({width*height} pixels)")
        
        # Конвертируем в RGB
        if image.mode != 'RGB':
            image = image.convert('RGB')
        
        # Получаем все пиксели в список
        pixels = list(image.getdata())
        
        # Открываем порт
        print(f"Opening port: {port} at {baudrate} baud")
        ser = serial.Serial(port, baudrate, timeout=5)
        time.sleep(2)
        
        # Очищаем буферы
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        
        print("Sending pixels in batches...")
        start_time = time.time()
        
        # Отправляем пакетами по 100 пикселей
        batch_size = 100
        pixel_count = 0
        
        for y in range(height):
            for x in range(width):
                idx = y * width + x
                r, g, b = pixels[idx]
                
                # Формируем команду
                command = f"disp_px {x} {y} {r} {g} {b}\r\n"
                ser.write(command.encode())
                pixel_count += 1
                
                # Небольшая задержка между командами
                if pixel_count % 100 == 0:
                    time.sleep(0.001)
            
            # Выводим прогресс
            if (y + 1) % 20 == 0:
                elapsed = time.time() - start_time
                percent = (y + 1) / height * 100
                print(f"Progress: {y + 1}/{height} rows ({percent:.1f}%) - {elapsed:.1f}s")
        
        elapsed = time.time() - start_time
        print(f"\nComplete! {pixel_count} pixels in {elapsed:.2f}s")
        print(f"Speed: {pixel_count/elapsed:.0f} px/sec")
        
        # Обновляем дисплей
        ser.write(b"disp_update\r\n")
        time.sleep(0.1)
        
    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()
    finally:
        if ser:
            time.sleep(0.1)
            ser.close()
            print("Port closed")

if __name__ == "__main__":
    image_path = '/home/zeverlanotebook/Загрузки/roronoa-zoro-one-3840x2160-10523.png'
    send_image_to_display_batch(image_path, max_width=240, max_height=240)