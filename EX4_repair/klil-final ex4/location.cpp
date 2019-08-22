Location* destination(string current_location, Direction direction) {
	assert(this->contains(current_location));

	try{
		string destination =  *((this->beginAt(current_location)).Move(direction));
	}
	catch(...) {//possible exception is KGraphIteratorReachedEnd
		return NULL;
	}
	return (*this)[destination];
}
