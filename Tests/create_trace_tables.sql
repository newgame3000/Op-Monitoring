--
-- PostgreSQL database dump
--

-- Dumped from database version 14.7 (Debian 1:14.7-3)
-- Dumped by pg_dump version 14.7 (Debian 1:14.7-3)

DROP SCHEMA trace_2025 CASCADE;

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


CREATE SCHEMA trace_2025;
SET default_tablespace = '';
SET default_table_access_method = heap;

CREATE SEQUENCE trace_2025.session_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE trace_2025.session (
    id integer DEFAULT nextval('trace_2025.session_id_seq'::regclass) NOT NULL,
    spo_name character varying(64) NOT NULL,
    login character varying(64) NOT NULL,
    server character varying(64) NOT NULL,
    start_time timestamp without time zone DEFAULT now() NOT NULL
);


CREATE TABLE trace_2025.span (
    session_id integer NOT NULL,
    id integer NOT NULL,
    parent_span_id integer,
    name character varying(256) NOT NULL,
    description text,
    context json
);


CREATE TABLE trace_2025.event (
    session_id integer NOT NULL,
    span_id integer NOT NULL,
    id integer NOT NULL,
    severity integer NULL,
    type integer NOT NULL,
    event_time timestamp without time zone DEFAULT now() NOT NULL,
    description text,
    add_info json
);

ALTER TABLE ONLY trace_2025.event
    ADD CONSTRAINT event_pkey PRIMARY KEY (session_id, span_id, id);

ALTER TABLE ONLY trace_2025.span
    ADD CONSTRAINT span_pkey PRIMARY KEY (session_id, id);

ALTER TABLE ONLY trace_2025.session
    ADD CONSTRAINT session_1_pkey PRIMARY KEY (id);

ALTER TABLE ONLY trace_2025.event
    ADD CONSTRAINT event_session_id_span_id_fkey FOREIGN KEY (session_id, span_id) REFERENCES trace_2025.span(session_id, id);

ALTER TABLE ONLY trace_2025.span
    ADD CONSTRAINT span_session_id_fkey FOREIGN KEY (session_id) REFERENCES trace_2025.session(id) NOT VALID;


GRANT USAGE ON SCHEMA trace_2025 TO PUBLIC;

GRANT SELECT,INSERT ON TABLE trace_2025.event TO PUBLIC;

GRANT SELECT,USAGE ON SEQUENCE trace_2025.session_id_seq TO PUBLIC;

GRANT SELECT,INSERT ON TABLE trace_2025.session TO PUBLIC;

GRANT SELECT,INSERT ON TABLE trace_2025.span TO PUBLIC;


--
-- PostgreSQL database dump complete
--

