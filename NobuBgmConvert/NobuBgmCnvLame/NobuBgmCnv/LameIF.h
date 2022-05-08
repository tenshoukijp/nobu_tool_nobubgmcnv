#pragma once

void CommandLame( char *szMySelf, char *szSrcFileName );
void CommandDecodeLame( char *szMySelf, char *szSrcFileName );

extern "C" {
	int lame_wmain(int argc, char *argv[]);
};