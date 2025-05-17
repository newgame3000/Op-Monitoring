TRUNCATE trace_2025.event CASCADE;
TRUNCATE trace_2025.span CASCADE;
TRUNCATE trace_2025.session CASCADE;
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (0, 'СПО 4', 'admin', 'main_server', '2024-02-22 16:41:13.636463');
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (1, 'СПО 2', 'admin', 'reserve_server', '2024-02-22 16:41:13.636463');
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (2, 'СПО 3', 'prog', 'main_server', '2024-02-22 16:49:33.636463');
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (3, 'СПО 1', 'prog', 'reserve_server', '2024-02-22 17:06:13.636463');
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (4, 'СПО 3', 'user1', 'main_server', '2024-02-22 17:31:13.636463');
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (5, 'СПО 3', 'user1', 'reserve_server', '2024-02-22 18:04:33.636463');
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (6, 'СПО 2', 'prog', 'reserve_server', '2024-02-22 18:46:13.636463');
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (7, 'СПО 4', 'admin', 'reserve_server', '2024-02-22 19:36:13.636463');
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (8, 'СПО 3', 'admin', 'main_server', '2024-02-22 20:34:33.636463');
INSERT INTO trace_2025.session ("id", "spo_name", "login", "server", "start_time") VALUES (9, 'СПО 3', 'prog', 'reserve_server', '2024-02-22 21:41:13.636463');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (2, 0, -1, 'Получение ИД(ИМП)', 'Получение исходных данных при формировании именованного массива параметров ', '{
"imp_name":"juk01",
"imp_type":"15"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (7, 1, 0, 'Формирование текста ИМП оператором', 'Формирование текста именованного массива параметров оператором', '{
"imp_name":"juk01",
"imp_type":"15"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (3, 2, 1, 'отправка готового ИМП', 'Отправка готового именованного массива параметров', '{
"imp_name":"juk01",
"imp_type":"15"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (7, 3, 2, 'Получение ИД(МЦИ)', 'Получение исходных данных при формировании массивов цифровой информации', '{
"mci_name":"pok87",
"mci_type":"4"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (9, 4, 3, 'Ввод атрибутов МЦИ', 'Ввод атрибутов при формировании массивов цифровой информации', '{
"mci_name":"pok87",
"mci_type":"4"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (6, 5, 4, 'Формирование текста МЦИ оператором', 'Формирование текста массивов цифровой информации оператором', '{
"mci_name":"pok87",
"mci_type":"4"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (0, 6, 5, 'Проверка МЦИ', 'Проверка массивов цифровой информации', '{
"mci_name":"pok87",
"mci_type":"4"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (9, 7, 6, 'Отправка готового МЦИ', 'Отправка готового массивов цифровой информации', '{
"mci_name":"pok87",
"mci_type":"4"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (0, 8, 7, 'Получение ИД(ПСС)', 'Получение исходных данных при формировании программы сеанса связи', '{
"pss_name":"e34",
"pss_type":"8"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (3, 9, 8, 'Ввод атрибутов ПСС', 'Ввод атрибутов при формировании программы сеанса связи', '{
"pss_name":"e34",
"pss_type":"8"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (2, 10, 9, 'Формирование текста ПСС оператором', 'Формирование текста программы сеанса связи оператором', '{
"pss_name":"e34",
"pss_type":"8"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (3, 11, 10, 'Отправка готовой ПСС', 'Отправка готовой программы сеанса связи', '{
"pss_name":"e34",
"pss_type":"8"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (4, 12, 11, 'Отправка МЦИ', 'Отправка массивов цифровой информации на отработку на моделирующих стендах', '{}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (8, 13, 12, 'Получение МЦИ с результатом', 'Получение массивов цифровой информации с результатом отработки на моделирующих стендах', '{}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (5, 14, 13, 'Получение ИД(Выдача КПИ)', 'Получение исходных данных при выдаче командно-программной информации', '{
"krl":"a-band",
"object":"83"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (3, 15, 14, 'Выдача команды', 'Выдача команды (командно-программной информации)', '{
"krl":"a-band",
"object":"83"
}');
INSERT INTO trace_2025.span ("session_id", "id", "parent_span_id", "name", "description", "context") VALUES (5, 16, 15, 'Отработка выполнения', 'Отработка выполнения команды (получение кваитанции)', '{
"krl":"a-band",
"object":"83"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (2, 0, 0, 0, 0, '2024-02-22 16:49:33.636463', 'Подключение к базе данных МЦИ при формировании ИМП', '{
"server_name":"reserve",
"db_name":"db3"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (2, 0, 1, 0, 1, '2024-02-22 16:49:38.636463', 'Подключение к базе данных ТОП при формировании ИМП', '{
"server_name":"reserve",
"db_name":"db1"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (2, 0, 2, 0, 2, '2024-02-22 16:49:43.636463', 'Подключение к базе данных БИ при формировании ИМП', '{
"server_name":"main",
"db_name":"db2"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (7, 1, 3, 0, 3, '2024-02-22 16:49:53.636463', 'Оператор создал/открыл текст ИМП', '{
"text_name":"tg5",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (7, 1, 4, 0, 4, '2024-02-22 16:49:58.636463', 'Оператор сохранил текст ИМП', '{
"text_name":"tg5",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (7, 1, 5, 0, 5, '2024-02-22 16:50:03.636463', 'Оператор отправил текст ИМП на трансляцию', '{
"text_name":"tg5",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (7, 1, 6, 0, 6, '2024-02-22 16:50:08.636463', 'Чтение ТОП при формировании ИМП', '{
"var_type":"r1",
"var_name":"store",
"ab":"brts"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (7, 1, 7, 0, 7, '2024-02-22 16:50:13.636463', 'Чтение БИ при формировании ИМП', '{
"var_type":"uu7",
"start_cond":"445",
"tg_num":"26",
"bf_num":"132",
"object":"1"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (7, 1, 8, 0, 8, '2024-02-22 16:50:18.636463', 'Трансляция ИМП завершена', '{
"text_name":"tg5",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 2, 9, 0, 9, '2024-02-22 16:50:28.636463', 'Оператор подтвердил запись ИМП в базу данных', '{
"imp_name":"juk01"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 2, 10, 0, 10, '2024-02-22 16:50:33.636463', 'Запись ИМП в базу данных МЦИ', '{
"imp_name":"juk01"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (7, 3, 11, 0, 11, '2024-02-22 19:37:23.636463', 'Подключение к базе данных ТОП при формировании МЦИ', '{
"server_name":"reserve",
"db_name":"db1"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (7, 3, 12, 0, 12, '2024-02-22 19:37:28.636463', 'Подключение к базе данных БИ при формировании МЦИ', '{
"server_name":"main",
"db_name":"db2"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (7, 3, 13, 0, 13, '2024-02-22 19:37:33.636463', 'Произошло подключение к базе данных МЦИ', '{
"server_name":"reserve",
"db_name":"db3"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (9, 4, 14, 0, 14, '2024-02-22 19:37:43.636463', 'Оператор ввел атрибуты при формировании МЦИ', '{
"mci_name":"pok87",
"mci_type":"4",
"ab":"brts",
"turn":"322",
"krl":"a-band",
"obj":"83"
,"sbi":"77"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (9, 4, 15, 0, 15, '2024-02-22 19:37:48.636463', 'Чтение БИ при формировании атрибутов МЦИ', '{
"var_type":"uu7",
"start_cond":"445",
"tg_num":"26",
"bf_num":"132",
"object":"1"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (9, 4, 16, 0, 16, '2024-02-22 19:37:53.636463', 'Проверка корректности атрибутов при формировании МЦИ', '{
"mci_name":"pok87",
"mci_type":"4",
"ab":"brts",
"turn":"322",
"krl":"a-band",
"obj":"83"
,"sbi":"77"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (6, 5, 17, 0, 17, '2024-02-22 19:38:03.636463', 'Оператор создал/открыл текст МЦИ', '{
"text_name":"kop78",
"text_type":"mim",
"ab":"brts"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (6, 5, 18, 0, 18, '2024-02-22 19:38:08.636463', 'Оператор сохранил текст МЦИ', '{
"text_name":"kop78",
"text_type":"mim",
"ab":"brts"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (6, 5, 19, 0, 19, '2024-02-22 19:38:13.636463', 'Оператор отправил текст МЦИ на трансляцию', '{
"text_name":"kop78",
"text_type":"mim",
"ab":"brts"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (6, 5, 20, 0, 20, '2024-02-22 19:38:18.636463', 'Чтение ТОП при формировании МЦИ', '{
"var_type":"r1",
"var_name":"store",
"ab":"brts"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (6, 5, 21, 0, 21, '2024-02-22 19:38:23.636463', 'Чтение БИ при формировании МЦИ', '{
"var_type":"uu7",
"start_cond":"445",
"tg_num":"26",
"bf_num":"132",
"object":"1"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (6, 5, 22, 0, 22, '2024-02-22 19:38:28.636463', 'Чтение ИМП при формировании МЦИ', '{
"imp_name":"juk01"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (6, 5, 23, 0, 23, '2024-02-22 19:38:33.636463', 'Трансляция МЦИ завершена', '{
"text_name":"kop78",
"text_type":"mim",
"ab":"brts",
"imp_name":"juk01"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (0, 6, 24, 0, 24, '2024-02-22 19:38:43.636463', 'Оператор отправил МЦИ на отработку', '{
"text_name":"kop78",
"text_type":"mim",
"ab":"brts"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (0, 6, 25, 0, 25, '2024-02-22 19:38:48.636463', 'Отработка МЦИ завершена', '{
"text_name":"kop78",
"text_type":"mim",
"ab":"brts"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (9, 7, 26, 0, 26, '2024-02-22 19:38:58.636463', 'Оператор подтвердил запись МЦИ в базу данных', '{
"text_name":"kop78",
"text_type":"mim",
"ab":"brts"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (9, 7, 27, 0, 27, '2024-02-22 19:39:03.636463', 'Запись МЦИ в базу данных', '{
"text_name":"kop78",
"text_type":"mim",
"ab":"brts"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (0, 8, 28, 0, 28, '2024-02-22 17:09:13.636463', 'Подключение к базе данных ПСС', '{
"server_name":"main",
"db_name":"db4"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (0, 8, 29, 0, 29, '2024-02-22 17:09:18.636463', 'Подключение к базе данных БИ при формировании ПСС', '{
"server_name":"main",
"db_name":"db2"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (0, 8, 30, 0, 30, '2024-02-22 17:09:23.636463', 'Подключение к базе данных МЦИ при формировании ПСС', '{
"server_name":"reserve",
"db_name":"db3"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 9, 31, 0, 31, '2024-02-22 17:09:33.636463', 'Оператор ввел атрибуты при формировании ПСС', '{
"pss_name":"e34",
"pss_type":"8",
"kis_nums":["101","102","103"],
"turn_nums":["322","23","54"],
"krl":"a-band",
"obj":"83"
,"sbi":"128"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 9, 32, 0, 32, '2024-02-22 17:09:38.636463', 'Чтение БИ при формировании атрибутов ПСС', '{
"var_type":"uu7",
"start_cond":"445",
"tg_num":"26",
"bf_num":"132",
"object":"1"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 9, 33, 0, 33, '2024-02-22 17:09:43.636463', 'Проверка корректности атрибутов при формировании ПСС', '{
"pss_name":"e34",
"pss_type":"8",
"kis_nums":["101","102","103"],
"turn_nums":["322","23","54"],
"krl":"a-band",
"obj":"83"
,"sbi":"128"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (2, 10, 34, 0, 34, '2024-02-22 17:09:53.636463', 'Оператор создал/открыл текст ПСС', '{
"text_name":"pss34",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (2, 10, 35, 0, 35, '2024-02-22 17:09:58.636463', 'Оператор сохранил текст ПСС', '{
"text_name":"pss34",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (2, 10, 36, 0, 36, '2024-02-22 17:10:03.636463', 'Оператор отправил текст ПСС на трансляцию', '{
"text_name":"pss34",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (2, 10, 37, 0, 37, '2024-02-22 17:10:08.636463', 'Чтение МЦИ, ДУ, КУ, СК, ФК', '{
"commands":[{
"type":"mci",
"name":"pok87"
}, {
"type":"ku",
"name":"02",
"turn":"322"
}, {
"type":"du",
"name":"55",
"turn":"322"
}]
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (2, 10, 38, 0, 38, '2024-02-22 17:10:13.636463', 'Трансляция ПСС завершена', '{
"text_name":"pss34",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 11, 39, 0, 39, '2024-02-22 17:10:23.636463', 'Оператор подтвердил запись ПСС в базу данных', '{
"text_name":"pss34",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 11, 40, 0, 40, '2024-02-22 17:10:28.636463', 'Запись ПСС в базу данных', '{
"text_name":"pss34",
"text_type":"kolos",
"ab":"br67"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (4, 12, 41, 0, 41, '2024-02-22 19:40:38.636463', 'Оператор отправил МЦИ/ПСС на отработку', '{
"type_obj":"mci"
,"name":"pok87",
"type":"4"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (4, 12, 42, 0, 42, '2024-02-22 19:40:43.636463', 'Получена транспортная квитанция об отправке МЦИ/ПСС на отработку', '{
"type_obj":"mci"
,"name":"pok87",
"type":"4"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (8, 13, 43, 0, 43, '2024-02-22 19:40:53.636463', 'Получена пользовательская квитанция о результате отработки МЦИ/ПСС', '{
"type_obj":"mci"
,"name":"pok87",
"type":"4"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (5, 14, 44, 0, 44, '2024-02-22 21:46:03.636463', 'Подключение к базе данных ПСС при выдаче КПИ', '{
"server_name":"main",
"db_name":"db4"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (5, 14, 45, 0, 45, '2024-02-22 21:46:08.636463', 'Подключение к базе данных МЦИ при выдаче КПИ', '{
"server_name":"reserve",
"db_name":"db3"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 15, 46, 0, 46, '2024-02-22 21:46:18.636463', 'Оператор зарезервировал КИС', '{
"kis_num":"1011"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 15, 47, 0, 47, '2024-02-22 21:46:23.636463', 'Оператор выбрал команду/ПСС', '{
"type_obj":"mci",
"name":"pok87",
"type":"4"
}');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (3, 15, 48, 0, 48, '2024-02-22 21:46:28.636463', 'Оператор выдал команду', '{ }');
INSERT INTO trace_2025.event ("session_id", "span_id", "id", "severity", "type", "event_time", "description", "add_info") VALUES (5, 16, 49, 0, 49, '2024-02-22 21:46:38.636463', 'Получение результата выполнения команды', '{
"type_obj":"mci",
"name":"pok87",
"type":"4"
}');
