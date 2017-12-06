from spacetrack import SpaceTrackClient


def main():
	st = SpaceTrackClient('bekor88@gmail.com', 'Rich4RdPhiliPhF3ynm4nn')
	ids = [25544, 37820, 41765]
	for i in ids:
		lines = st.tle_latest(iter_lines=True, epoch='>now-30', limit=1, orderby='epoch', norad_cat_id=i, format='tle')
		with open('satellites/' + str(i) + '.dat', 'w') as fp:
			for line in lines:
				fp.write(line + "\n")
			
if __name__ == "__main__":
	main()