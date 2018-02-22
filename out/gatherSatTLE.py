from spacetrack import SpaceTrackClient


def main():
	st = SpaceTrackClient('username', 'pw')
	ids = [25544, 20580, 27540, 28485, 31135,\
		  33053, 37820, 41765, 40889, 40890,\
		  41175, 41174, 41550, 41549, 41859,\
		  41860, 41861, 41862, 23118, 28378]
	lines = st.tle_latest(iter_lines=True, epoch='>now-30', orderby='epoch', norad_cat_id=ids, format='tle')
	satDic = {}
	for id in ids:
		satDic[id] = []
	for line in lines:
		for id in ids:
			if str(id) in line:
				satDic[id].append(line)
				
	for id, line in satDic.items():
		with open('satellites/' + str(id) + '.dat', 'w') as fp:
			if not len(line) < 2:
				fp.write(line[-1] + "\n")
				fp.write(line[-2] + "\n")
			else:
				fp.write("\n")
			
if __name__ == "__main__":
	main()
