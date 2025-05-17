import random
import datetime

random.seed(10)

def fill_span_and_event(add_info_var, context, step):

    #Заполнение таблицы span
    column_names_2 = ['session_id', 'id', 'parent_span_id', 'name', 'description', 'context']

    name_var = [ ['Получение ИД(ИМП)', 'Формирование текста ИМП оператором', 'отправка готового ИМП'],
            ['Получение ИД(МЦИ)', 'Ввод атрибутов МЦИ', 'Формирование текста МЦИ оператором', 'Проверка МЦИ', 'Отправка готового МЦИ'],
            ['Получение ИД(ПСС)', 'Ввод атрибутов ПСС', 'Формирование текста ПСС оператором', 'Отправка готовой ПСС'],
            ['Отправка МЦИ', 'Получение МЦИ с результатом'],
            ['Получение ИД(Выдача КПИ)', 'Выдача команды', 'Отработка выполнения']]

    description_var = [ ['Получение исходных данных при формировании именованного массива параметров ', 'Формирование текста именованного массива параметров оператором', 'Отправка готового именованного массива параметров'],

            ['Получение исходных данных при формировании массивов цифровой информации', 'Ввод атрибутов при формировании массивов цифровой информации',
                'Формирование текста массивов цифровой информации оператором', 'Проверка массивов цифровой информации', 'Отправка готового массивов цифровой информации'],

            ['Получение исходных данных при формировании программы сеанса связи', 'Ввод атрибутов при формировании программы сеанса связи', 'Формирование текста программы сеанса связи оператором', 'Отправка готовой программы сеанса связи'],

            ['Отправка массивов цифровой информации на отработку на моделирующих стендах', 'Получение массивов цифровой информации с результатом отработки на моделирующих стендах'],

            ['Получение исходных данных при выдаче командно-программной информации', 'Выдача команды (командно-программной информации)', 'Отработка выполнения команды (получение кваитанции)']]
    #
    # context = [' ']

    columns_str_2 = ', '.join(f'"{column}"' for column in column_names_2)

    spans_session_id = {}

    id = 0
    for j in range(5):
        for k in range(len(name_var[j])):

            session_id = random.randint(0, count_1 - 1)
            spans_session_id[id] = session_id

            values = []
            values.append(session_id)
            values.append(id * step)
            values.append(id * step - 1)
            values.append(f"'{name_var[j][k]}'")
            values.append(f"'{description_var[j][k]}'")
            values.append(f"'{context[j]}'")

            values_str_2 = ', '.join(str(value) for value in values)

            insert_query = f'INSERT INTO trace_2025.{table_name_2} ({columns_str_2}) VALUES ({values_str_2});\n'
            f.write(insert_query)
            id += 1


    #Заполнение таблицы event
    column_names_3 = ['session_id', 'span_id', 'id', 'severity', 'type', 'event_time', 'description', 'add_info']

    # severity = ['Ошибка программы', 'Ошибка пользователя', 'Предупреждение', 'Информация']

    description_var_event = [
                        [
                            ['Подключение к базе данных МЦИ при формировании ИМП', 'Подключение к базе данных ТОП при формировании ИМП', 'Подключение к базе данных БИ при формировании ИМП'],
                            ['Оператор создал/открыл текст ИМП', 'Оператор сохранил текст ИМП', 'Оператор отправил текст ИМП на трансляцию', 'Чтение ТОП при формировании ИМП', 'Чтение БИ при формировании ИМП', 'Трансляция ИМП завершена'],
                            ['Оператор подтвердил запись ИМП в базу данных', 'Запись ИМП в базу данных МЦИ']
                        ],

                        [
                            ['Подключение к базе данных ТОП при формировании МЦИ', 'Подключение к базе данных БИ при формировании МЦИ', 
                            'Произошло подключение к базе данных МЦИ'],
                            ['Оператор ввел атрибуты при формировании МЦИ', 'Чтение БИ при формировании атрибутов МЦИ', 'Проверка корректности атрибутов при формировании МЦИ'],
                            ['Оператор создал/открыл текст МЦИ', 'Оператор сохранил текст МЦИ', 'Оператор отправил текст МЦИ на трансляцию', 'Чтение ТОП при формировании МЦИ', 'Чтение БИ при формировании МЦИ', 'Чтение ИМП при формировании МЦИ', 'Трансляция МЦИ завершена'],
                            ['Оператор отправил МЦИ на отработку', 'Отработка МЦИ завершена'],
                            ['Оператор подтвердил запись МЦИ в базу данных','Запись МЦИ в базу данных']
                        ],

                        [
                            ['Подключение к базе данных ПСС', 'Подключение к базе данных БИ при формировании ПСС', 'Подключение к базе данных МЦИ при формировании ПСС'],
                            ['Оператор ввел атрибуты при формировании ПСС', 'Чтение БИ при формировании атрибутов ПСС', 'Проверка корректности атрибутов при формировании ПСС'],
                            ['Оператор создал/открыл текст ПСС', 'Оператор сохранил текст ПСС', 'Оператор отправил текст ПСС на трансляцию', 'Чтение МЦИ, ДУ, КУ, СК, ФК', 'Трансляция ПСС завершена'],
                            ['Оператор подтвердил запись ПСС в базу данных', 'Запись ПСС в базу данных']
                        ],

                        [
                            ['Оператор отправил МЦИ/ПСС на отработку', 'Получена транспортная квитанция об отправке МЦИ/ПСС на отработку'],
                            ['Получена пользовательская квитанция о результате отработки МЦИ/ПСС']
                        ],

                        [
                            ['Подключение к базе данных ПСС при выдаче КПИ', 'Подключение к базе данных МЦИ при выдаче КПИ'],
                            ['Оператор зарезервировал КИС', 'Оператор выбрал команду/ПСС', 'Оператор выдал команду'],
                            ['Получение результата выполнения команды']
                        ]
                    ]
    columns_str_3 = ', '.join(f'"{column}"' for column in column_names_3)
    id = 0
    id_span = 0

    delta_span = 5
    delta_event = 5

    for j in range(5):
        for k in range(len(name_var[j])):
            for t in range(len(description_var_event[j][k])):
                values = []
                values.append(spans_session_id[id_span])
                values.append(id_span)
                values.append(id * step)
                # values.append(random.randint(0, 3))

                # if j == 4 and k == 2:
                #    values.append(2) 
                # else:
                values.append(0)

                values.append(id)

                session_time = session_starts_time[spans_session_id[j]]

                # start_time = datetime.datetime(2025, 1, 1)
                # end_time = datetime.datetime(2025, 12, 31, 23, 59, 59, 999999)
                #
                # random_date = start_time + (end_time - start_time) * random.random()


                random_date = session_time + datetime.timedelta(0, delta_span * id_span) + datetime.timedelta(0, id * delta_event)

                empty = " "
                values.append(f"'{random_date.isoformat(empty)}'")

                # if j == 4 and k == 2:
                #     values.append(f"'Ошибка выдачи МЦИ, не совпала контрольная сумма пакета'") 
                #     str1 = '{\n"type_obj":"mci"\n,"name":"pok87",\n"type":"4",\n"error_code":"47"}'
                #     values.append(f"'{str1}'")
                # else:
                values.append(f"'{description_var_event[j][k][t]}'")
                values.append(f"'{add_info_var[add_info_map[j][k][t]]}'")

                values_str_3 = ', '.join(str(value) for value in values)

                insert_query = f'INSERT INTO trace_2025.{table_name_3} ({columns_str_3}) VALUES ({values_str_3});\n'
                f.write(insert_query)
                id += 1
            id_span += 1

    f.close()




filename = 'test_data.sql'

table_name_1 = 'session'
table_name_2 = 'span'
table_name_3 = 'event'

with open(filename, 'w') as f:

    f.write(f"TRUNCATE trace_2025.{table_name_3} CASCADE;\n")
    f.write(f"TRUNCATE trace_2025.{table_name_2} CASCADE;\n")
    f.write(f"TRUNCATE trace_2025.{table_name_1} CASCADE;\n")

    #Заполнение таблицы session
    column_names_1 = ['id', 'spo_name', 'login', 'server', 'start_time']

    spo_name_var = ['СПО 1', 'СПО 2', 'СПО 3', 'СПО 4']
    login_var = ['user1', 'admin', 'prog']
    server_var = ['main_server', 'reserve_server']

    count_1 = 10
    session_starts_time = {}

    start_time = datetime.datetime(2023, 1, 1)
    end_time = datetime.datetime(2024, 12, 31, 23, 59, 59, 999999)

    random_date = start_time + (end_time - start_time) * random.random()
    delta_session = 500

    columns_str_1 = ', '.join(f'"{column}"' for column in column_names_1)
    for i in range(count_1):
        values = []
        values.append(i)
        values.append(f"'{random.choice(spo_name_var)}'")
        values.append(f"'{random.choice(login_var)}'")
        values.append(f"'{random.choice(server_var)}'")

        session_starts_time[i] = random_date

        empty = " "
        values.append(f"'{random_date.isoformat(empty)}'")

        values_str_1 = ', '.join(str(value) for value in values)

        insert_query = f'INSERT INTO trace_2025.{table_name_1} ({columns_str_1}) VALUES ({values_str_1});\n'
        f.write(insert_query)

        random_date +=  datetime.timedelta(0, delta_session * i)

    add_info_map = [
                        [
                            [5, 0, 2],
                            [4, 4, 4, 1, 3, 4],
                            [6, 6]
                        ],

                        [
                            [0, 2, 5],
                            [7, 3, 7],
                            [8, 8, 8, 1, 3, 6, 9],
                            [8, 8],
                            [8, 8]
                        ],

                        [
                            [11, 2, 5],
                            [14, 3, 14],
                            [10, 10, 10, 15, 10],
                            [10, 10]
                        ],

                        [
                            [16, 16],
                            [16]
                        ],

                        [
                            [11, 5], 
                            [12, 13, 17],
                            [13]
                        ]

                    ]

    add_info_var = [
                        ['{\n"server_name":"reserve",\n"db_name":"db1"\n}', '{\n"var_type":"r1",\n"var_name":"store",\n"ab":"brts"\n}',

                         '{\n"server_name":"main",\n"db_name":"db2"\n}', '{\n"var_type":"uu7",\n"start_cond":"445",\n"tg_num":"26",\n"bf_num":"132",\n"object":"1"\n}',

                         '{\n"text_name":"tg5",\n"text_type":"kolos",\n"ab":"br67"\n}',  '{\n"server_name":"reserve",\n"db_name":"db3"\n}',

                         '{\n"imp_name":"juk01"\n}', '{\n"mci_name":"pok87",\n"mci_type":"4",\n"ab":"brts",\n"turn":"322",\n"krl":"a-band",\n"obj":"83"\n,"sbi":"77"\n}',

                         '{\n"text_name":"kop78",\n"text_type":"mim",\n"ab":"brts"\n}', '{\n"text_name":"kop78",\n"text_type":"mim",\n"ab":"brts",\n"imp_name":"juk01"\n}',

                         '{\n"text_name":"pss34",\n"text_type":"kolos",\n"ab":"br67"\n}', '{\n"server_name":"main",\n"db_name":"db4"\n}',

                         '{\n"kis_num":"1011"\n}', '{\n"type_obj":"mci",\n"name":"pok87",\n"type":"4"\n}',

                         '{\n"pss_name":"e34",\n"pss_type":"8",\n"kis_nums":["101","102","103"],\n"turn_nums":["322","23","54"],\n"krl":"a-band",\n"obj":"83"\n,"sbi":"128"\n}',

                         '{\n"commands":[{\n"type":"mci",\n"name":"pok87"\n}, {\n"type":"ku",\n"name":"02",\n"turn":"322"\n}, {\n"type":"du",\n"name":"55",\n"turn":"322"\n}]\n}',

                         '{\n"type_obj":"mci"\n,"name":"pok87",\n"type":"4"\n}', '{ }']
                   ]


    context = [
                        ['{\n"imp_name":"juk01",\n"imp_type":"15"\n}',
                         '{\n"mci_name":"pok87",\n"mci_type":"4"\n}',
                         '{\n"pss_name":"e34",\n"pss_type":"8"\n}',
                         '{}',
                         '{\n"krl":"a-band",\n"object":"83"\n}']
                   ]


    for i in range(len(add_info_var)):
        fill_span_and_event(add_info_var[i], context[i], i + 1)
















