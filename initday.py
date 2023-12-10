import datetime, os, sys, urllib.request, urllib.error
from dotenv import load_dotenv

load_dotenv()

COOKIE = os.getenv("COOKIE")
VCPROJ = "aoc.vcxproj"
VCFILTERS = VCPROJ + ".filters"


def make_cpp(day):
    cppfilename = f"day{day}.cpp"
    if os.path.exists(cppfilename):
        print(f"not creating {cppfilename} as it already exists")
        return False

    with open("template.cpp", "rt") as templfile:
        templ = templfile.read()
        cpp = templ.replace('N', str(day))
        with open(cppfilename, "wt") as cppfile:
            cppfile.write(cpp)

    return True


def try_download_input(day):
    if len(COOKIE.strip()) == 0:
        print("no login cookie; not downloading input")
        return ""
    
    req = urllib.request.Request(f"https://adventofcode.com/2023/day/{day}/input", headers={'Cookie': COOKIE})

    try:
        with urllib.request.urlopen(req) as response:
            input = response.read().decode("utf-8")
            print(f"downloaded input: '{input[:30]}...'")
            return input

    except urllib.error.HTTPError as e:
        print(f"http error fetching input: {e.code}")
        print("   " + e.read().decode("utf-8"))
        print("assuming that's all fine, just ignoring it")
        return ""
    
    except ConnectionRefusedError as e:
        print("AoC server refused the connection. have you been spamming?")
        return ""
    

def make_data(day):
    input = try_download_input(day)

    datafilename = f"data/day{day}.txt"
    if os.path.exists(datafilename):
        print(f"not creating {datafilename} as it already exists")
        return False
    
    with open(datafilename, "wt") as datafile:
        datafile.write(input)

    return True


def add_to_vcproj(day, cpp, data):
    print("fangling the vcproj")

    with open(VCPROJ, "rt") as vcprojfile:
        vcprojlines = vcprojfile.readlines()

    if cpp:
        # insert cpp file
        for ix,line in enumerate(vcprojlines):
            if "<ClCompile Include" in line:
                vcprojlines.insert(ix, f"    <ClCompile Include=\"day{day}.cpp\" />\n")
                break

    if data:
        # insert data file
        for ix,line in enumerate(vcprojlines):
            if "<Text Include" in line:
                vcprojlines.insert(ix, f"    <Text Include=\"data\day{day}.txt\" />\n")
                break

    with open(VCPROJ, "wt") as vcprojfile:
        vcprojfile.writelines(vcprojlines)


def add_to_vcproj_filters(day, cpp, data):
    print("fangling the vcproj [filters]")

    with open(VCFILTERS, "rt") as vcfiltersfile:
        vcfilterslines = vcfiltersfile.readlines()

    if cpp:
        # insert cpp file
        for ix,line in enumerate(vcfilterslines):
            if "<ClCompile Include" in line:
                vcfilterslines.insert(ix,   f"    <ClCompile Include=\"day{day}.cpp\">\n")
                vcfilterslines.insert(ix+1,  "      <Filter>Source Files</Filter>\n")
                vcfilterslines.insert(ix+2,  "    </ClCompile>\n")
                break

    if data:
        # insert data file
        for ix,line in enumerate(vcfilterslines):
            if "<Text Include" in line:
                vcfilterslines.insert(ix,   f"    <Text Include=\"data\day{day}.txt\">\n")
                vcfilterslines.insert(ix+1,  "      <Filter>data</Filter>\n")
                vcfilterslines.insert(ix+2,  "    </Text>\n")
                break

    with open(VCFILTERS, "wt") as vcfiltersfile:
        vcfiltersfile.writelines(vcfilterslines)



def main():
    if len(sys.argv) == 2:
        day = int(sys.argv[1])
    else:
        today = datetime.date.today()
        day = today.day

    print(f"preparing files for day {day} december")

    created_cpp = make_cpp(day)
    created_data = make_data(day)

    if created_cpp or created_data:
        add_to_vcproj(day, created_cpp, created_data)
        add_to_vcproj_filters(day, created_cpp, created_data)


if __name__ == '__main__':
    main()
