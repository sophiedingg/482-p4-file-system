/ (type d) (inode block 0)
	owner: 
	size: 1
	data disk blocks: 2 
	entry 0: anand, inode block 1
	entry 1: sophie, inode block 3
	entry 2: david, inode block 4

/anand (type d) (inode block 1)
	owner: user1
	size: 1
	data disk blocks: 6 
	entry 0: anand, inode block 5
	entry 1: file1, inode block 7

/anand/anand (type d) (inode block 5)
	owner: user1
	size: 0
	data disk blocks: 

/anand/file1 (type f) (inode block 7)
	owner: user1
	size: 1
	data disk blocks: 16 
/0
345677890;We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Gover

/sophie (type d) (inode block 3)
	owner: user1
	size: 1
	data disk blocks: 9 
	entry 0: sophie, inode block 8

/sophie/sophie (type d) (inode block 8)
	owner: user1
	size: 1
	data disk blocks: 13 
	entry 0: file1, inode block 12
	entry 1: file2, inode block 14

/sophie/sophie/file1 (type f) (inode block 12)
	owner: user1
	size: 0
	data disk blocks: 


/sophie/sophie/file2 (type f) (inode block 14)
	owner: user1
	size: 1
	data disk blocks: 17 
/0
345677890;We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Gover

/david (type d) (inode block 4)
	owner: user1
	size: 1
	data disk blocks: 11 
	entry 0: david, inode block 10
	entry 1: file1, inode block 15

/david/david (type d) (inode block 10)
	owner: user1
	size: 0
	data disk blocks: 

/david/file1 (type f) (inode block 15)
	owner: user1
	size: 0
	data disk blocks: 


4078 disk blocks free
