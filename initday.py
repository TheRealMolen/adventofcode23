import sys, urllib.request


def make_cpp(day):
    with open("template.cpp", "rt") as templfile:
        templ = templfile.read()
        cpp = templ.replace('N', str(day))
        with open(f"day{day}.cpp", "wt") as cppfile:
            cppfile.write(cpp)


def try_download_input(day):
    try:
        req = urllib.request.Request(f"https://adventofcode.com/2023/day/{day}/input")


def make_data(day):
    input = try_download_input(day)
    with open(f"data/day{day}.txt", "wt") as datafile:
        datafile.write(input)


def main():
    if len(sys.argv) != 2:
        print("usage: initday.py <day>")
        sys.exit(1)

    day = int(sys.argv[1])

    print(f"preparing files for day {day}")

    make_cpp(day)
    make_data(day)


if __name__ == '__main__':
    main()
