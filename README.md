# Лабораторна робота 4 — модуль ядра `hello`

Модуль з параметром `count` (uint), який задає, скільки разів надрукувати
`Hello, world!`. Перед кожним друком час події (`ktime_get()`) зберігається
у зв'язному списку; при вивантаженні модуль друкує час кожної події в
наносекундах і звільняє пам'ять.

| Значення `count` | Поведінка |
|---|---|
| не задано | 1 привітання |
| 0 | попередження, привітань немає, модуль завантажується |
| 1–4 | відповідна кількість привітань |
| 5–10 | попередження + відповідна кількість привітань |
| > 10 | помилка, `insmod` повертає `Invalid parameters` (-EINVAL) |

## Збирання (крос-компіляція для BBB)

```sh
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
export KDIR=$HOME/repos/linux-stable
make
$KDIR/scripts/checkpatch.pl -f hello.c   # має бути 0 errors, 0 warnings
```

Для QEMU використовуйте ті самі `export`, що й при збиранні ядра.

## Перевірка на платі

```sh
modinfo hello.ko

insmod hello.ko
cat /sys/module/hello/parameters/count
rmmod hello

insmod hello.ko count=0
rmmod hello

insmod hello.ko count=3
rmmod hello

insmod hello.ko count=7
cat /sys/module/hello/parameters/count
rmmod hello

insmod hello.ko count=15      # очікувано: Invalid parameters
dmesg | tail -n 30
```
