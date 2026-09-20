#!/data/data/com.termux/files/usr/bin/bash

bsd() {
    if [ $# -eq 0 ]; then
        echo "Usage: bsd file.c [options]  или  bsd file.cpp [options]" >&2
        return 1
    fi

    local file="$1"
    shift
    local options="$@"

    case "$file" in
        *.c)
            compiler="gcc"
            ;;
        *.cpp|*.cxx)
            compiler="g++"
            ;;
        *)
            echo "Ошибка: неподдерживаемый тип файла. Используйте .c или .cpp" >&2
            return 1
            ;;
    esac

    $compiler "$file" -o ~/test.out $options && ~/test.out && rm ~/test.out
}

file="shrine.c"

while true; do
    # hx "$file"

    bsd "$file" -lncurses

    read -p "Open file again (Y/n): " inp
    case "$inp" in
        n|N) break ;;
        *) ;;
    esac
done
