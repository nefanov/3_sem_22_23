# Как запустить
    make start
## Важно
Не прирывайте программы через ^С,это может сломать их при следующих запусках.
# Замеры
## 4GB
![4gb](https://github.com/NikitaAgeev/3_sem_22_23/blob/main/task3/4000.png)
## 400MB
![400mb](https://github.com/NikitaAgeev/3_sem_22_23/blob/main/task3/400.png)
## 40MB
![40mb](https://github.com/NikitaAgeev/3_sem_22_23/blob/main/task3/40.png)
## Выводы
Как можно видеть, передача коротких файлов настолько быстрая, что накладные расходы запуска программы настолько велики,что время работы практически не меняется от буфера (кроме SHM)
Также видно, что из-за синхронизации SHM работаетдольше всех.
Также MQ работает лучше FIFO, но при малых размерах буфера FIFO лучше.