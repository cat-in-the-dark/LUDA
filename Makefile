SHELL := /bin/bash

.PHONY: all
all: build run

.PHONY: reload
reload:
	cmake -Bcmake-build-debug

.PHONY: build
build: reload
	cmake --build cmake-build-debug -j 4

.PHONY: clean
clean:
	rm -rf cmake-build-debug

.PHONY: run
run:
	./cmake-build-debug/LUDA

.PHONY: run_game
run_game:
	./cmake-build-debug/LUDA game/main.lua

.PHONY: reload_web
reload_web:
	emcmake cmake -Bcmake-build-web -DPLATFORM=Web

.PHONY: build_web
build_web: reload_web
	cmake --build cmake-build-web -j 4

.PHONY: run_web
run_web:
	python -m http.server -d cmake-build-web/ 3000

.PHONY: clean_web
clean_web:
	rm -rf cmake-build-web

.PHONY: clear_web
clear_web:
	rm -f cmake-build-web/LUDA.wasm cmake-build-web/LUDA.html cmake-build-web/LUDA.js
