import re
from datetime import datetime, timedelta
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

# Данные из сообщения (скопированы как многострочная строка)
data_text = """ for time"""

# Регулярное выражение для извлечения времени и значения
pattern = re.compile(r'(\d{2})\s*:\s*(\d{2})\s*:\s*(\d{2})\s+([\d.]+)')

# Базовая дата (несущественная, нужна только для временной шкалы)
base_date = datetime(2026, 2, 14)

data = []
for line in data_text.strip().split('\n'):
    match = pattern.search(line)
    if match:
        h, m, s, val = match.groups()
        # Создаём datetime с базовой датой
        dt = base_date.replace(hour=int(h), minute=int(m), second=int(s))
        # Корректировка для времени после полуночи (если час меньше 22, значит это уже следующий день)
        if int(h) < 22:
            dt += timedelta(days=1)
        data.append((dt, float(val)))

# Фильтрация по нужному интервалу
start_time = base_date.replace(hour=22, minute=58, second=0)
end_time = base_date.replace(hour=23, minute=38, second=0)
# Для времени после полуночи тоже добавим день, но в нашем интервале всё до 23:38, поэтому коррекция не нужна
filtered_data = [(t, v) for t, v in data if start_time <= t <= end_time]

if not filtered_data:
    print("Нет данных в выбранном интервале.")
else:
    # Разделяем на x (время) и y (сопротивление)
    times, values = zip(*filtered_data)

    # Построение графика
    plt.figure(figsize=(12, 6))
    plt.plot(times, values, linewidth=1, color='blue')
    plt.title('Зависимость сопротивления от времени (22:58 – 23:38)')
    plt.xlabel('Время')
    plt.ylabel('Сопротивление, Ом')
    plt.grid(True, linestyle='--', alpha=0.7)

    # Форматирование оси времени
    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
    plt.gca().xaxis.set_major_locator(mdates.MinuteLocator(interval=5))
    plt.gcf().autofmt_xdate()  # Поворот меток для читаемости

    plt.tight_layout()
    plt.show()