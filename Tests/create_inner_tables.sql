DROP SCHEMA inner_db CASCADE;

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

CREATE SCHEMA inner_db;

SET default_tablespace = '';
SET default_table_access_method = heap;

CREATE TABLE inner_db.mci (
    id SERIAL NOT NULL,
    id_res_create integer,
    id_send integer,
    id_res_test integer,
    id_operator integer NOT NULL,
    id_imp integer,
    name character varying(64) NOT NULL,
    type character varying(64) NOT NULL,
    abonent character varying(64),
    turn integer,
    krl character varying(64),
    object character varying(64),
    sbi integer,
    last_save_source_time timestamp without time zone DEFAULT NULL,
    translation_time timestamp without time zone DEFAULT NULL
);

CREATE TABLE inner_db.imp (
    id SERIAL NOT NULL,
    name character varying(64) NOT NULL,
    type character varying(64) NOT NULL,
    abonent character varying(64),
    last_save_source_time timestamp without time zone DEFAULT NULL,
    translation_time timestamp without time zone DEFAULT NULL,
    id_res_create integer,
    id_operator integer NOT NULL
);

CREATE TABLE inner_db.result (
    id SERIAL NOT NULL,
    res boolean NOT NULL,
    error_code integer,
    description character varying(256)
);

CREATE TABLE inner_db.operator (
    id SERIAL NOT NULL,
    login character varying(64) NOT NULL,
    server character varying(64) NOT NULL,
    start_time timestamp without time zone NOT NULL
);

CREATE TABLE inner_db.pss (
    id SERIAL NOT NULL,
    id_operator integer NOT NULL,
    id_res_create integer,
    id_send integer,
    id_res_test integer,
    last_save_source_time timestamp without time zone DEFAULT NULL,
    translation_time timestamp without time zone DEFAULT NULL,
    name character varying(64) NOT NULL,
    type character varying(64) NOT NULL,
    kis_nums integer array, 
    turn_nums integer array, 
    krl character varying(64),
    object character varying(64),
    sbi integer,
    structure json
);

CREATE TABLE inner_db.send (
    id SERIAL NOT NULL,
    id_operator integer NOT NULL,
    id_res_send integer,
    krl character varying(64),
    send_time timestamp without time zone DEFAULT NULL,
    object character varying(64),
    kis integer
);

CREATE TABLE inner_db.command (
    id SERIAL NOT NULL,
    id_send integer,
    num integer NOT NULL,
    turn integer NOT NULL,
    type character varying(64) NOT NULL
);


ALTER TABLE ONLY inner_db.mci
    ADD CONSTRAINT mci_pkey PRIMARY KEY (id);

ALTER TABLE ONLY inner_db.imp
    ADD CONSTRAINT imp_pkey PRIMARY KEY (id);

ALTER TABLE ONLY inner_db.result
    ADD CONSTRAINT result_pkey PRIMARY KEY (id);

ALTER TABLE ONLY inner_db.operator
    ADD CONSTRAINT operator_pkey PRIMARY KEY (id);

ALTER TABLE ONLY inner_db.command
    ADD CONSTRAINT command_pkey PRIMARY KEY (id);

ALTER TABLE ONLY inner_db.send
    ADD CONSTRAINT send_pkey PRIMARY KEY (id);

ALTER TABLE ONLY inner_db.pss
    ADD CONSTRAINT pss_pkey PRIMARY KEY (id);


ALTER TABLE ONLY inner_db.mci
    ADD CONSTRAINT id_res_create_fkey FOREIGN KEY (id_res_create) REFERENCES inner_db.result(id);

ALTER TABLE ONLY inner_db.mci
    ADD CONSTRAINT id_send_fkey FOREIGN KEY (id_send) REFERENCES inner_db.send(id);

ALTER TABLE ONLY inner_db.mci
    ADD CONSTRAINT id_res_test_fkey FOREIGN KEY (id_res_test) REFERENCES inner_db.result(id);

ALTER TABLE ONLY inner_db.mci
    ADD CONSTRAINT id_operator_fkey FOREIGN KEY (id_operator) REFERENCES inner_db.operator(id);

ALTER TABLE ONLY inner_db.mci
    ADD CONSTRAINT id_imp_fkey FOREIGN KEY (id_imp) REFERENCES inner_db.imp(id);


ALTER TABLE ONLY inner_db.imp
    ADD CONSTRAINT id_res_create_fkey FOREIGN KEY (id_res_create) REFERENCES inner_db.result(id);

ALTER TABLE ONLY inner_db.imp
    ADD CONSTRAINT id_operator_fkey FOREIGN KEY (id_operator) REFERENCES inner_db.operator(id);


ALTER TABLE ONLY inner_db.send
    ADD CONSTRAINT id_operator_fkey FOREIGN KEY (id_operator) REFERENCES inner_db.operator(id);

ALTER TABLE ONLY inner_db.send
    ADD CONSTRAINT id_res_send_fkey FOREIGN KEY (id_res_send) REFERENCES inner_db.result(id);


ALTER TABLE ONLY inner_db.command
    ADD CONSTRAINT id_send_fkey FOREIGN KEY (id_send) REFERENCES inner_db.send(id);


ALTER TABLE ONLY inner_db.pss
    ADD CONSTRAINT id_operator_fkey FOREIGN KEY (id_operator) REFERENCES inner_db.operator(id);

ALTER TABLE ONLY inner_db.pss
    ADD CONSTRAINT id_res_create_fkey FOREIGN KEY (id_res_create) REFERENCES inner_db.result(id);

ALTER TABLE ONLY inner_db.pss
    ADD CONSTRAINT id_send_fkey FOREIGN KEY (id_send) REFERENCES inner_db.send(id);

ALTER TABLE ONLY inner_db.pss
    ADD CONSTRAINT id_res_test_fkey FOREIGN KEY (id_res_test) REFERENCES inner_db.result(id);


GRANT USAGE ON SCHEMA inner_db TO PUBLIC;

GRANT SELECT,INSERT ON TABLE inner_db.mci TO PUBLIC;

GRANT SELECT,INSERT ON TABLE inner_db.imp TO PUBLIC;

GRANT SELECT,INSERT ON TABLE inner_db.result TO PUBLIC;

GRANT SELECT,INSERT ON TABLE inner_db.operator TO PUBLIC;

GRANT SELECT,INSERT ON TABLE inner_db.command TO PUBLIC;
