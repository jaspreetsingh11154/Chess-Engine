import subprocess

class UCIEngine:
	# spawns the process and set up the pipes
	def __init__(self, executable_path, name = "Engine"):
		self.name = name
		self.process = subprocess.Popen(
			[executable_path],
			stdin = subprocess.PIPE,
			stdout = subprocess.PIPE,
			stderr = subprocess.STDOUT,
			text=True
		)
	def send_command(self, command):
		# sending command and fulshing the buffer 
		self.process.stdin.write(f"{command}\n")
		self.process.stdin.flush()

	def read_until(self, expected_token):
		#reads line unitl the end token found like -> uciok, readyok
		lines = []
		#print(f"<---[{self.name}]--->")
		while True:
			#readline reads unitl it found the new line character or "\r\n" character
			line = self.process.stdout.readline()
			if not line:
				return_code = self.process.poll()
				raise RuntimeError(
					f"Engine '{self.name}' terminated unexpectedly",
					f"with return code {return_code} while waiting for '{expected_token}'"
				)
				break
			line =  line.strip()
			#print(f"{line}")
			lines.append(line.strip())
			if expected_token in line:
				return lines

	def quit(self):
		self.send_command("quit")
		self.process.terminate()

def parse_divide_output(raw_output):
	move_map = {}
	for line in raw_output:
		line = line.strip()
		#skip empty lines
		if not line or ":" not in line or "Nodes searched" in line or "info" in line:
			continue
		#split at the rear most 
		parts = line.rsplit(":", 1)
		#parts
		move = parts[0].strip()
		node_str = parts[1].strip()

		if move and node_str:
			move_map[move] = node_str

	return move_map

def Bisect_perft(teacher, student, fen, depth, move_history=None):
	if move_history is None:
		move_history = []

	if depth == 0:
		print(f"\n-----------------------------------------------------------------")
		print(f"| [Error position reacher] | Depth: {depth}")
		print(f"| position <FEN>: '{fen}' ")
		print(f"| Move History: {' '.join(move_history)} ")
		print(f"-----------------------------------------------------------------\n")
		return move_history
	#structure the fen posiotion
	if move_history:
		position_cmd = f"position fen {fen} moves {' '.join(move_history)}"
	else:
		position_cmd = f"position fen {fen}"

	teacher.send_command(position_cmd)
	student.send_command(position_cmd)
	student.send_command("print board")
	for line in student.read_until("board printed"):
		print(f"{line}")

	teacher.send_command(f"go perft {depth}")
	student.send_command(f"go perft {depth}")
	t_output = teacher.read_until("Nodes searched: ")
	s_output = student.read_until("Nodes searched: ")
	t_map = parse_divide_output(t_output)
	s_map = parse_divide_output(s_output)
	#stores the all unique keys/moves
	all_moves = sorted(set(t_map.keys()).union(set(s_map.keys())))
	for move in all_moves:
		t_nodes = t_map.get(move, 0)
		s_nodes = s_map.get(move, 0)

		if t_nodes != s_nodes:
			print(f"Mismatch on move {move} at Depth {depth}! Stockfish: {t_nodes}, Student: {s_nodes}")
			return Bisect_perft(teacher, student, fen, depth - 1, move_history + [move])
	print(f"All moves matched at Depth {depth} | Moves: {' '.join(move_history)}")
	return True


def run_perft_test(teacher, student, fen_command, depth):
	#fen Initializing on boath
	#fen_command = fen_command.strip(";")[0]
	teacher.send_command(f"position fen {fen_command}")
	student.send_command(f"position fen {fen_command}")
	#print(f"fen-> {fen_command}")
	#perft on both
	teacher.send_command(f"go perft {depth}")
	student.send_command(f"go perft {depth}")
	#read output
	t_output = teacher.read_until("Nodes searched: ")
	s_output = student.read_until("Nodes searched: ")
	#store the node count for comparision
	t_nodes = int(t_output[-1].split(":")[-1].strip())
	s_nodes = int(s_output[-1].split(":")[-1].strip())
	#verify match
	diff = abs(t_nodes - s_nodes)
	if diff == 0:
		print("RESULT: [ PASS ] NODE counts match perfectly!")
		return True
	else:
		print(f"RESULT: [ FAIL ] Mismatch detected! Difference: {diff:,}")
		print("\n-----Running Bisect_perft (Locating Mismatch Branch)-----")
		Bisect_perft(teacher, student, fen, depth)
		return False
		 

def get_FEN(filename):
	with open(filename, "r" ) as file:
		for line in file:
			clean_line = line.strip()
			#skip empty lines and comments
			if clean_line and not clean_line.startswith('#'):
				yield clean_line.split(";")[0].strip()

#---------------------------------------[MANIN]------------------------------
#teacher and student
student_path = r"C:\DEV\Chess\out\build\x64-debug\Chess.exe"
teacher_path = r"C:\Users\hp\Downloads\stockfish-windows-x86-64-avx2\stockfish\stockfish-windows-x86-64-avx2.exe"

print("--------Initializing teacher(stockfish)-------")
teacher = UCIEngine(teacher_path, "Teacher/stockfish")
teacher.send_command("uci")
teacher.read_until("uciok")
teacher.send_command("isready")
teacher.read_until("readyok")

print("--------Initializing student(MyEngine)-------")
student = UCIEngine(student_path, "student/MyEngine")
student.send_command("uci")
student.read_until("uciok")
student.send_command("isready")
student.read_until("readyok")

try:
	# go perft Testing with fen positons 
	for fen in get_FEN("positions.txt"):
		run_perft_test(teacher, student, fen, 4)
finally:
	print("All Test completed")
	#cleanup process
	teacher.quit()
	student.quit()