/ (type d) (inode block 0)
	owner: 
	size: 1
	data disk blocks: 2 
	entry 0: file, inode block 1
	entry 1: read_dir, inode block 3
	entry 2: write_dir, inode block 7

/file (type f) (inode block 1)
	owner: user1
	size: 0
	data disk blocks: 


/read_dir (type d) (inode block 3)
	owner: user1
	size: 1
	data disk blocks: 5 
	entry 0: slow, inode block 4

/read_dir/slow (type f) (inode block 4)
	owner: user1
	size: 1
	data disk blocks: 6 
We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying

/write_dir (type d) (inode block 7)
	owner: user1
	size: 1
	data disk blocks: 9 
	entry 0: file, inode block 8

/write_dir/file (type f) (inode block 8)
	owner: user1
	size: 0
	data disk blocks: 


4086 disk blocks free
