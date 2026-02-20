import numpy as np
import matplotlib.pyplot as plt
from numpy.polynomial import Polynomial

# ==================== Данные из лабораторной работы ====================
# Пустой калориметр
R_cal = [18.195, 18.245, 18.295, 18.345, 18.395, 18.445, 18.495, 18.545,
         18.595, 18.645, 18.695, 18.745, 18.795, 18.845, 18.895, 18.945]
t_cal = [0, 42.32, 80.45, 126.46, 174.72, 224.01, 274.66, 327.45,
         382.05, 438.53, 495.88, 554.69, 614.43, 676.19, 742.21, 808.79]

# Железный образец
R_iron = [18.195, 18.245, 18.295, 18.345, 18.395, 18.445, 18.495, 18.545,
          18.595, 18.645, 18.695, 18.745, 18.795, 18.845, 18.895, 18.945]
t_iron = [0, 66.39, 136.06, 208.42, 283.76, 361.06, 441.09, 522.17,
          606.01, 692.47, 780.83, 870.19, 961.58, 1057.7, 1155.36, 1253.24]

# Алюминиевый образец
R_alu = [18.245, 18.295, 18.345, 18.395, 18.445, 18.495, 18.545, 18.595,
         18.645, 18.695, 18.745, 18.795, 18.845, 18.895, 18.945]
t_alu = [49.93, 104.69, 165.09, 218.57, 293.63, 361.07, 429.88, 501.65,
         575.48, 651.30, 725.99, 804.81, 885.21, 969.57, 1053.04]

R0 = 18.120          # Ом, начальное сопротивление при комнатной температуре

# ==================== Функция для вычисления производной ====================
def compute_derivative(R, t):
    """Возвращает массивы R_mid (начальное сопротивление интервала) и dR/dt."""
    dRdt = []
    R_start = []
    for i in range(len(R)-1):
        dR = R[i+1] - R[i]
        dt = t[i+1] - t[i]
        dRdt.append(dR / dt)
        R_start.append(R[i])      # производная приписывается к начальному R
    return np.array(R_start), np.array(dRdt)

# Вычисляем производные для каждого случая
R_cal_d, dRdt_cal = compute_derivative(R_cal, t_cal)
R_iron_d, dRdt_iron = compute_derivative(R_iron, t_iron)
R_alu_d, dRdt_alu = compute_derivative(R_alu, t_alu)

# ==================== Квадратичная аппроксимация dR/dt(R) ====================
def fit_quadratic(R_vals, dRdt_vals):
    p = Polynomial.fit(R_vals, dRdt_vals, 2)
    # переводим в обычные коэффициенты (для удобства)
    c = p.convert()
    return c

coef_cal = fit_quadratic(R_cal_d, dRdt_cal)
coef_iron = fit_quadratic(R_iron_d, dRdt_iron)
coef_alu = fit_quadratic(R_alu_d, dRdt_alu)

print("Коэффициенты квадратичной аппроксимации dR/dt = a*R^2 + b*R + c")
print(f"Калориметр: a={coef_cal.coef[2]:.3e}, b={coef_cal.coef[1]:.3e}, c={coef_cal.coef[0]:.3e}")
print(f"Железо:     a={coef_iron.coef[2]:.3e}, b={coef_iron.coef[1]:.3e}, c={coef_iron.coef[0]:.3e}")
print(f"Алюминий:   a={coef_alu.coef[2]:.3e}, b={coef_alu.coef[1]:.3e}, c={coef_alu.coef[0]:.3e}")

# Вычисляем значение при R0
dRdt_cal_at_R0 = coef_cal(R0)
dRdt_iron_at_R0 = coef_iron(R0)
dRdt_alu_at_R0 = coef_alu(R0)
print(f"\nЗначения dR/dt при R0 = {R0} Ом (в 1e-4 Ом/с):")
print(f"Калориметр: {dRdt_cal_at_R0*1e4:.1f}")
print(f"Железо:     {dRdt_iron_at_R0*1e4:.1f}")
print(f"Алюминий:   {dRdt_alu_at_R0*1e4:.1f}")

# ==================== Построение графиков ====================
plt.style.use('seaborn-v0_8-whitegrid')

# ---- 1. График R(t) для всех случаев ----
plt.figure(figsize=(10, 6))
plt.plot(t_cal, R_cal, 'o-', label='Пустой калориметр', markersize=4)
plt.plot(t_iron, R_iron, 's-', label='Железо', markersize=4)
plt.plot(t_alu, R_alu, '^-', label='Алюминий', markersize=4)
plt.axhline(y=R0, color='gray', linestyle='--', linewidth=1, label=f'$R_0 = {R0}$ Ом')
plt.xlabel('Время, с')
plt.ylabel('Сопротивление, Ом')
plt.title('Зависимость сопротивления от времени нагрева')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)
plt.tight_layout()
plt.savefig('results_of_measuring.png', dpi=300)
plt.show()

# ---- 2. Графики dR/dt(R) с аппроксимацией ----
# Для построения плавной кривой аппроксимации создадим массив R
R_fit = np.linspace(18.1, 19.0, 200)

# Калориметр
plt.figure(figsize=(8, 5))
plt.plot(R_cal_d, dRdt_cal*1e4, 'o', label='Экспериментальные точки', markersize=5)
plt.plot(R_fit, coef_cal(R_fit)*1e4, '-', label='Квадратичная аппроксимация')
plt.axvline(x=R0, color='gray', linestyle='--', label=f'$R_0 = {R0}$ Ом')
plt.xlabel('Сопротивление $R$, Ом')
plt.ylabel('$dR/dt$, $10^{-4}$ Ом/с')
plt.title('Калориметр: зависимость $dR/dt(R)$')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)
plt.tight_layout()
plt.savefig('dR_dt(r)_for_calorimetr.png', dpi=300)
plt.show()

# Железо
plt.figure(figsize=(8, 5))
plt.plot(R_iron_d, dRdt_iron*1e4, 'o', label='Экспериментальные точки', markersize=5)
plt.plot(R_fit, coef_iron(R_fit)*1e4, '-', label='Квадратичная аппроксимация')
plt.axvline(x=R0, color='gray', linestyle='--', label=f'$R_0 = {R0}$ Ом')
plt.xlabel('Сопротивление $R$, Ом')
plt.ylabel('$dR/dt$, $10^{-4}$ Ом/с')
plt.title('Железный образец: зависимость $dR/dt(R)$')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)
plt.tight_layout()
plt.savefig('dR_dt(r)_for_steel.png', dpi=300)
plt.show()

# Алюминий
plt.figure(figsize=(8, 5))
plt.plot(R_alu_d, dRdt_alu*1e4, 'o', label='Экспериментальные точки', markersize=5)
plt.plot(R_fit, coef_alu(R_fit)*1e4, '-', label='Квадратичная аппроксимация')
plt.axvline(x=R0, color='gray', linestyle='--', label=f'$R_0 = {R0}$ Ом')
plt.xlabel('Сопротивление $R$, Ом')
plt.ylabel('$dR/dt$, $10^{-4}$ Ом/с')
plt.title('Алюминиевый образец: зависимость $dR/dt(R)$')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)
plt.tight_layout()
plt.savefig('dR_dt(r)_for_aluminium.png', dpi=300)
plt.show()