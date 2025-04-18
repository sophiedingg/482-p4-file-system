/ (type d) (inode block 0)
	owner: 
	size: 1
	data disk blocks: 2 
	entry 0: dir, inode block 1

/dir (type d) (inode block 1)
	owner: user1
	size: 1
	data disk blocks: 4 
	entry 0: file, inode block 3
	entry 1: anand, inode block 5

/dir/file (type f) (inode block 3)
	owner: user1
	size: 0
	data disk blocks: 


/dir/anand (type d) (inode block 5)
	owner: user1
	size: 1
	data disk blocks: 7 
	entry 0: david, inode block 6

/dir/anand/david (type d) (inode block 6)
	owner: user1
	size: 1
	data disk blocks: 9 
	entry 0: sophie, inode block 8
	entry 1: david, inode block 10

/dir/anand/david/sophie (type d) (inode block 8)
	owner: user1
	size: 0
	data disk blocks: 

/dir/anand/david/david (type f) (inode block 10)
	owner: user1
	size: 1
	data disk blocks: 12 
We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying

4084 disk blocks free
