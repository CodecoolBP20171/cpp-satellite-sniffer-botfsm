import ephem
import sys

def main():
	sat = ephem.readtle(sys.argv[1], sys.argv[2], sys.argv[3])
	sat.compute(sys.argv[4]);
	print("%s %s" % (float(sat.sublong), float(sat.sublat)))

if __name__ == "__main__":
	main()