#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
//using namespace std;

//#include "debug.h"

class BoardHelper {
public:
static int **allocateBoard( int boardSize ) {
    //int **board = new int*[boardSize];
//    for( int i = 0; i < boardSize; i++ ) {
//        board[i] = new int[boardSize];
//        for( int j = 0; j < boardSize; j++ ) {
//            board[i][j] = 0;
//        }
//    }
    int *contiguousarray = new int[ boardSize * boardSize ];
    memset(contiguousarray, 0, sizeof(int) * boardSize * boardSize );
    int **board = new int*[boardSize];
    for( int i = 0; i < boardSize; i++ ) {
        board[i] = &(contiguousarray[i*boardSize]);
    }
    return board;
}

static float **allocateFloats( int boardSize ) {
    //int **board = new int*[boardSize];
//    for( int i = 0; i < boardSize; i++ ) {
//        board[i] = new int[boardSize];
//        for( int j = 0; j < boardSize; j++ ) {
//            board[i][j] = 0;
//        }
//    }
    float *contiguousarray = new float[ boardSize * boardSize ];
    memset(contiguousarray, 0, sizeof(float) * boardSize * boardSize );
    float **board = new float*[boardSize];
    for( int i = 0; i < boardSize; i++ ) {
        board[i] = &(contiguousarray[i*boardSize]);
    }
    return board;
}

static void deleteBoard( int ***p_board, int boardSize ) {
   if( p_board == 0 ) {
      return;
   }
   delete[] (*p_board)[0];
   delete[] *p_board;
   *p_board = 0;
}

static void deleteBoard( float ***p_board, int boardSize ) {
   if( p_board == 0 ) {
      return;
   }
   delete[] (*p_board)[0];
   delete[] *p_board;
   *p_board = 0;
}

static void copyBoard( int const*const *const src, int *const*const dst, int boardSize ) {
    for( int i = 0; i < boardSize; i++ ) {
        for( int j = 0; j < boardSize; j++ ) {
            if( dst[i][j] != src[i][j] ) {
                dst[i][j] = src[i][j];
            }
        }
    }
}

static void wipeBoard( int *const*const board, int boardSize ) {
    for( int i = 0; i < boardSize; i++ ) {
        for( int j = 0; j < boardSize; j++ ) {
            board[i][j] = 0;
        }
    }
}

//static void printInts( int const*const*const board, int boardSize ) {
//    ostringstream ss;
//    ss << "\n";
//    for( int i = 0; i < boardSize; i++ ) {
//       for( int j = 0; j < boardSize; j++ ) {
//          ss << board[i][j] << " ";
//       }
//       ss << "\n";
//    }
//    debug( ss.str() );
//}

//static void printBoard( int const *const *const board, int boardSize ) {
///*    int numdigits = 1;
//    for( int i = 0; i < boardSize; i++ ) {
//       for( int j = 0; j < boardSize; j++ ) {
//          string thisnum = toString( board[i][j] );
//          int thisdigits = thisnum.length();
//          numdigits = thisdigits > numdigits ? thisdigits : numdigits;
//       }
//    }*/
//    ostringstream ss;
//    ss << "\n";
//    for( int i = 0; i < boardSize; i++ ) {
//       for( int j = 0; j < boardSize; j++ ) {
//          if( board[i][j] == 0 ) {
//              ss << ".";
//          }
//          if( board[i][j] == 1 ) {
//              ss << "*";
//          }
//          if( board[i][j] == 2 ) {
//              ss << "O";
//          }
//          if( board[i][j] == 3 ) {
//              ss << "+";
//          }
//       }
//       ss << "\n";
//    }
//    debug( ss.str() );
//}

static void loadBoard( int ***p_board, string filepath, int *p_boardSize, int *p_piecesPlaced ) {
    ifstream f;
    f.open( filepath.c_str() );
    //f >> boardSize;
    //int **board = 0;
   string thisline;
   f >> thisline;
   *p_boardSize = thisline.length();
   if( *p_boardSize == 0 ) {
      cout << "boardhelper::loadBoard. error: boardsize 0, " << filepath << endl;
      throw "boardhelper::loadBoard. error: boardsize 0 " + filepath;
   }
   //cout << "boardsize: " << boardSize << endl;
   *p_board = allocateBoard( *p_boardSize );
    for( int i = 0; i < *p_boardSize; i++ ) {
       if( i == 0 ) {
       }
       for( int j = 0; j < *p_boardSize; j++ ) {
          string thischar = string("") + thisline[j];
          if( thischar == "*" ) {
              (*p_board)[i][j] = 1;
              (*p_piecesPlaced)++;
          }
          if( thischar == "O" ) {
              (*p_board)[i][j] = 2;
              (*p_piecesPlaced)++;
          }
       }
       f >> thisline;
    }
    f.close();
//    return board;
}
};

