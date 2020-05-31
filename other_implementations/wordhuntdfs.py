import json;
class WordHunt(object):

	dictionary = {};
	ans = [];
	paths = [];


	def make_dict(self):
		f = open('dictionaries/470kwords.txt', 'r');
		fr = f.read().splitlines();
		for word in fr:
			self.dictionary[word] = '1';

		with open('dict.json', 'w') as outfile:
			json.dump(self.dictionary, outfile);

		
	def recurse(self, board, row, col, word, path, visited):
		if row < 0 or row >= 4 or col < 0 or col >= 4:
			return;

		if visited[4*row + col]:
			return;

		word = word + board[4*row + col];
		visited[4*row + col] = True;
		
		#Check if word is in dict
		if(word in self.dictionary and len(word) > 2):
			self.ans.append(word);
			self.paths.append(path);


		

		for x, y, d in [1, 0, ", D"], [0, 1, ", R"], [-1, 0, ", U"], [0, -1, ", L"], [1, -1, ", LD"], [-1, 1, ", RU"], [1, 1, ", RD"], [-1, -1, ", LU"]:
			if row+x >= 0 and row+x < 4 and col+y >= 0 and col+y < 4:
				if not visited[4*(row+x) + col+y]:
					self.recurse(board, row+x, col+y, word, path+d, visited);

		visited[4*row + col] = False;


	def solve(self):
		self.make_dict();
		read = input("Enter the board: ");

		read = read.lower();

		board = read[0:16];

		visited = [ False for x in range(4*len(board) + len(board[0])) ];

		for x in range(len(board)):
			for y in range(len(board)):
				self.recurse(board, x, y, "", "(" + str(x) + ", " + str(y) + ")",  [ False for x in range(4*len(board) + len(board[0])) ]);

		pair = zip(self.ans, self.paths);

		final = sorted(pair, key=lambda x: len(x[0]), reverse=True);
		for i in range(len(final)):
			print(i, final[i][0]);
			print(" ", final[i][1]);
		
		
wh = WordHunt();
wh.solve();



