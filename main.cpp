#include<iostream>

#include "core.hpp"
#include "ossSocket.hpp"
#include "client/command.hpp"
#include "client/commandFactory.hpp"
#include "client/edb.hpp"


using namespace emeralddb; 
int main(int argc, char** argv) {
	client::Edb edb; 
	edb.start(); 
	return 0; 
}