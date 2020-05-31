import json;
class WordHunt(object):

	dictionary = {};
	ans = [];
	paths = [];

	def __init__(self):
		
		f = open('words_dictionary.json');
		self.dictionary = json.load(f);
		f.close();

	def make_dict(self):
		f = open('words.txt', 'r');
		fr = f.read().splitlines();
		for word in fr:
			self.dictionary[word] = '1';

		with open('dictionaries/dict.json', 'w') as outfile:
			json.dump(self.dictionary, outfile);

		
	def recurse(self, board, row, col, word, path, visited):
		if row < 0 or row >= 4 or col < 0 or col >= 4:
			return;

		if visited[4*row + col]:
			return;

		word = word + board[4*row + col];
		newVisited = visited.copy();
		newVisited[4*row + col] = True;
		
		#check if word is in dict
		if(word in self.dictionary and len(word) > 2):
			print(word);
			self.ans.append(word);
			self.paths.append(path);


		

		for x, y, d in [1, 0, ", D"], [0, 1, ", R"], [-1, 0, ", U"], [0, -1, ", L"], [1, -1, ", LD"], [-1, 1, ", RU"], [1, 1, ", RD"], [-1, -1, ", LU"]:
			if row+x >= 0 and row+x < 4 and col+y >= 0 and col+y < 4:
				if not visited[4*(row+x) + col+y]:
					self.recurse(board, row+x, col+y, word, path+d, newVisited);

	def solve(self):
		read = input("Enter the board: ");
		read = read.lower();
		board = read[0:16];

		visited = [ False for x in range(4*len(board) + len(board[0])) ];

		for x in range(len(board)):
			for y in range(len(board)):
				self.recurse(board, x, y, "", "(" + str(x) + ", " + str(y) + ")",  visited);

		

		pair = zip(self.ans, self.paths);

		final = sorted(pair, key=lambda x: len(x[0]), reverse=True);

		for i in range(len(final)):
			print(i, final[i][0]);
			print(" ", final[i][1]);		
		
wh = WordHunt();
wh.solve();



