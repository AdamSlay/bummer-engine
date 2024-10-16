.PHONY: build clean run run-only

build: build.sh
	./build.sh

clean:
	rm -rf build/ vcpkg/ vcpkg_installed/

run: build
	./build/BummerEngine

run-only:
	./build/BummerEngine