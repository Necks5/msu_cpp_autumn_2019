import subprocess

def check(expr, result, code):
	check.counter += 1
	out = subprocess.run(['./a.out', expr], stdout = subprocess.PIPE)
	if(out.returncode != code):
		print('return incorrect value in test {0}'.format(check.counter))
		return
	elif(code == 1):
		print('test {0} passed'.format(check.counter))
		return
	if (out.stdout != result):
		print('error {0} != {1}, got {2} in test{3}'.format(expr, result, out.stdout, check.counter))
		return
	print('test {0} passed'.format(check.counter))

check.counter = 0
check('2 + 2', b'4', 0)
check('2 + 3', b'5', 0)
check('2 * 5 + 6 * 2', b'22', 0)
check('10 / 2 / 5', b'1', 0)
check('2 +     6*  -1', b'-4', 0)
check('2 * 6 * 2', b'24', 0)
check('-1 * -1 * -4 / -2', b'2', 0)
check('2+4*', b'1', 1)
check('2 *', b'2', 1)
check('2 * 2 *', b'4', 1)
check('2 + 2 *', b'4', 1)
