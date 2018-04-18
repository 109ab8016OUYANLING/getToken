#include <stdio.h>
#include <string.h>


enum TokenType { IDENTIFIER = 34512, CONSTANT = 87232, SPECIAL = 29742 } ;


typedef char * CharPtr ;

struct Column {
  int column ;                  // ��token(���r��)�b��line���X�{��column
  Column * next;                // �U�@�Ӧ�token(�b��line)���X�{��column
} ; // Column

typedef Column * ColumnPtr ;


struct Line {                   // struct Line �O���F4�ظ�T
  int line ;                    // ��token��b���@ line
  ColumnPtr firstAppearAt ;     // ���즹token�Ĥ@���X�{�b��line�����@column
  ColumnPtr lastAppearAt ;      // ���즹token�̫�X�{�b��line�����@column
  Line *  next;                 // �P�@ token �U�@���X�{�b���@ line
} ;
typedef Line * LinePtr ;

struct Token {                  // Struct token �����F4�ظ�T
  CharPtr tokenStr ;            // tokenStr ��A���U�Ӫ�token
  TokenType type;               // type ������token�O�ݩ���@��case
  LinePtr firstAppearOn ;       // �� token �Ĥ@���X�{�b���@ line
  LinePtr lastAppearOn ;        // �� token �̫�X�{�b���@ line
  Token * next ;                // ���V�U�@��token
} ; // struct Token


typedef Token * TokenPtr ;

TokenPtr gFront = NULL, gRear = NULL ;  // ���O���VToken��C���Y���

typedef char Str100[ 100 ] ; // �����O�O����Ψӫŧi�@�Ӱ}�C�ܼơB�H��Ū�r�ꤧ��

int gLine = 1 ;              // �u�U�@�ӭnŪ�i�Ӫ��r���v�Ҧb��line number

int gColumn = 0 ;            // �u�U�@�ӭnŪ�i�Ӫ��r���v�Ҧb��column number
void Assort(char ch);
void InsertColumn( ColumnPtr & firstAppearAt, ColumnPtr & lastAppearAt ) ;
void InsertLine( LinePtr & firstAppearOn, LinePtr & lastAppearOn, LinePtr temp );
void InsertToken( Str100 name, int kind, LinePtr temp1 );
void PrintColandLine( LinePtr temp, ColumnPtr temp2);
void SkipComment ( char & comment );

void Getchar(char & ch) {
	scanf("%c", &ch );
	if ( ch == '\n' ) {
	  gColumn = 0;
	  gLine++ ;
	} // if
	else 
	  gColumn++;
} // Getchar

bool WhiteSpace( char ch ) {
	// check if white space
	if ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' )
	  return true;
	else 
	  return false;
} // WhiteSpace



bool Comment( char ch ) {
	// check if comment
	
	char latter ;
    if ( ch == '/') {
	  latter = ch ;
	  Getchar(ch);
	  if ( ch == '/' ) {
	    SkipComment(ch);
	    return true;
      } // if
      else if ( ch == '*') {
      	SkipComment(ch);
      	return true;
	  } // else if
	  else {
	    gColumn--;
	    Assort(latter);
        gColumn++; 
        if ( ! WhiteSpace(ch) )
	      Assort(ch);	    
      } // else
	} // if	 	
    return false;
} // Comment 

void SkipComment ( char & comment ) {  
	// comment means which comment  '//' or '/*'
	bool skip = false ; 
	char ch ;
	if ( comment == '/' ) {
	  while( ch != '\n' ) 
	    Getchar(ch);
	} // if 
	else { // '/*'
	  bool end = false;
	  
	  while( ! end ) {
	  	skip = false;
	    while ( ch == '*' ) {
	      Getchar(ch) ;
	      skip = true ; 
		} // while 
	    if (!skip)
	      Getchar(ch);
	      if ( ch == '/' )
	        end = true; 
	      
	  } // while
     
    } // else 
} // SkipComment.

void Skipchar(char & ch) {
	char letter = '\0' ;
	if ( WhiteSpace(ch) ) { // content \n \t ' '
	  Getchar(ch);
	  Skipchar(ch);
	} // if 
	else if ( Comment(ch) ) {
	  Getchar(ch);
	  Skipchar(ch);
	} // else if

} // Skipchar

bool case1(char ch){
	if ( ( 'A' <= ch && ch <= 'Z') || ( 'a' <= ch && ch <= 'z') ) 
	  return true;
	else 
	  return false;
} // case1

bool case2(char ch){
	if (ch >= '0' && ch <= '9' ) 
	  return true ;
	  
	else if ( ch == '\"') {
	  return true;
	} // else if 
	
	else if ( ch == '\'') {
	  return true;
	} // else if 
	
	else if ( ch == '.') {
	  return true;
	} // else if
	
	else 
	  return false;
	
} // case2

bool case3( char ch ) { //

  	if (case1(ch) && case2(ch) )
  	  return false;
  	else 
  	  return true;
} // case3

void Assort(char ch) {
  Str100 temp = {};
  LinePtr temp1 = new Line ;
  temp1 -> line = gLine;
  temp1 -> firstAppearAt = NULL ;
  temp1 -> lastAppearAt = NULL ;
  temp1 -> next = NULL ;
  bool done = false ;
  bool skip = false ;
  int i = 1; 
  int kind = 0 ;
  char input[2] = {};

  if (case1(ch)){
  	kind = 1;
    while( case1(ch) || ch == '_' || ( ch >= '0' && ch <= '9' ) ) {	
      input[0] = ch;  
      strcat( temp, input ); 
      InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
      Getchar(ch);
	} // while
    InsertToken( temp, kind, temp1 );
    //PrintColandLine( temp1, temp1-> firstAppearAt );
    if ( ! WhiteSpace(ch) ) 
	  Assort(ch); 
  } // if
  
  else if (case2(ch)) {
  	kind = 2;

  	if (ch >= '0' && ch <= '9' ) {
  		
  		while( ( ch >= '0' && ch <= '9' ) || ch == '.' ) {
	      input[0] = ch;
          strcat( temp, input );
          InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
          Getchar(ch);
        } // while
      if ( ! WhiteSpace(ch) && ! Comment(ch) ) 
	    Assort(ch); 
	} // if 
    else if (ch == '.') {
      char latter = ch;
	  input[0] = ch;
      strcat( temp, input);
      InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
	  Getchar(ch);
	  if (ch >= '0' && ch <= '9' ) {
	    while( ch >= '0' && ch <= '9' ) {
	      input[0] = ch;
          strcat( temp, input);
          InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
          Getchar(ch);
        } // while
      if ( ! WhiteSpace(ch) && ! Comment(ch) ) 
	    Assort(ch); 
	  } // 	if
	  else { // . ���ݩ�case2 so -> case3
	    kind = 3;
	    InsertToken( temp, kind, temp1 );  
		done = true;
		if ( ! WhiteSpace(ch) && ! Comment(ch) && ch != '\n' ) {
		  Assort(ch);
		} // if 
	  } // else // case3
    } // if
    else if (ch == '\'') {
      input[0] = ch;
      strcat( temp, input);
      InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
      Getchar(ch);
	  while( ch != '\'') {
	  	skip = false ; 
        if ( ch == '\\' ) { // �p�G�O �˱׽u�r�� ��U�@�Ӧr���ٲ�(Ū�F�����s �����˴�)  
          input[0] = ch;          
          strcat( temp, input);
          InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        // �H�U���U�@�Ӧr�� 
          Getchar(ch);
          input[0] = ch;
          strcat( temp, input);
          InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        // �H�W
		 
          skip = true; 
    	
        } // if
        
        if ( ! skip ) {
	      input[0] = ch;
          strcat( temp, input);
          InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
    	} // if
		 
        Getchar(ch);	  
	  } // while
	  
	  input[0] = ch;
      strcat( temp, input);
      InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
      //printf("%s\n", temp );
      //printf("(%d,%d)", temp1->line, temp1 ->firstAppearAt ->column );
	} // else if 
	
    else if (ch == '\"') {
      input[0] = ch;
      strcat( temp, input);
      InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
      Getchar(ch);
	  while( ch != '\"') {
	  	skip = false ; 
        if ( ch == '\\' ) { // �p�G�O �˱׽u�r�� ��U�@�Ӧr���ٲ�(Ū�F�����s �����˴�)  
          input[0] = ch;          
          strcat( temp, input);
          InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        // �H�U���U�@�Ӧr�� 
          Getchar(ch);
          input[0] = ch;
          strcat( temp, input);
          InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        // �H�W
		 
          skip = true; 
    	
        } // if
        
        if ( ! skip ) {
	      input[0] = ch;
          strcat( temp, input);
          InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
    	} // if
		 
        Getchar(ch);	  
	  } // while
	  
	  input[0] = ch;
      strcat( temp, input);
      InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
      //printf("%s\n", temp );
      //printf("(%d,%d)", temp1->line, temp1 ->firstAppearAt ->column );
	} // else if 
	
	// PrintColandLine( temp1, temp1-> firstAppearAt );
	
	if ( ! done )
	  InsertToken( temp, kind, temp1 );
    
  } // else if // case 2
  
  else if ( ! WhiteSpace(ch) ) { // case3
  
    
    input[0] = ch;
    strcat( temp, input);
    InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
    kind = 3;
	//-------------------------------------�H�Ucase3 ---------------------------------//
	
    if ( ch == '>') {  // >> >= 
      Getchar(ch);
      if ( ! Comment(ch) && ! WhiteSpace(ch) ) {
        InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        if (ch == '>')
          temp[1] = '>';
        else if (ch == '=')
          temp[1] = '='; 
        else 
          Assort(ch);
      } // if 
     
    } // if
    else if ( ch == '<') {  // << <=
      Getchar(ch);
      if ( ! Comment(ch) && ! WhiteSpace(ch) ) {
        InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        if (ch == '<')
          temp[1] = '<';
        else if (ch == '=')
          temp[1] = '='; 
        else 
          Assort(ch);
      } // if 
    } // if
    else if ( ch == '+') {  // ++
      Getchar(ch);
      if ( ! Comment(ch) && ! WhiteSpace(ch) ) {
        InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        if (ch == '+')
          temp[1] = '+'; 
        else 
          Assort(ch);
      } // if 
    } // if
    else if ( ch == '-') {  // -- ->
      Getchar(ch);
      if ( ! Comment(ch) && ! WhiteSpace(ch) ) {
        InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        if (ch == '-')
          temp[1] = '-'; 
        else if ( ch == '>' )
          temp[1] = '>';
        else 
          Assort(ch);
      } // if 
    } // if
    else if ( ch == '!') {  // !=
      Getchar(ch);
      if ( ! Comment(ch) && ! WhiteSpace(ch) ) {
        InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        if (ch == '=')
          temp[1] = '='; 
        else 
          Assort(ch);
      } // if 
    } // if
    else if ( ch == '=') {  // ==
      Getchar(ch);
      if ( ! Comment(ch) && ! WhiteSpace(ch) ) {
        InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        if (ch == '=')
          temp[1] = '='; 
        else 
          Assort(ch);
      } // if 
    } // if
    else if ( ch == '&') {  // &&
      Getchar(ch);
      if ( ! Comment(ch) && ! WhiteSpace(ch) ) {
        InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        if (ch == '&')
          temp[1] = '&'; 
        else 
          Assort(ch);
      } // if 
    } // if
    
    else if ( ch == '|') {  // ||
      Getchar(ch);
      if ( ! Comment(ch) && ! WhiteSpace(ch) ) {
        InsertColumn( temp1 -> firstAppearAt, temp1 -> lastAppearAt );
        if (ch == '|')
          temp[1] = '|'; 
        else 
          Assort(ch);
      } // if 
    } // if
	

	InsertToken( temp, kind, temp1 );
	
  } // else 
  
} // Assort   


void InsertColumn( ColumnPtr & firstAppearAt, ColumnPtr & lastAppearAt ) {
	ColumnPtr temp = new Column ;
	temp -> column = gColumn ;
	temp -> next = NULL;
	if ( firstAppearAt == NULL ) { // �Ĥ@���i�� 
	  firstAppearAt = temp ;
	  lastAppearAt = temp ;
	} // if 
	else { // �ĤG���H�� 
	  lastAppearAt -> next = temp;
	  lastAppearAt = temp ; 
	}  // else	
} // InsertColumn 

void InsertLine( LinePtr & firstAppearOn, LinePtr & lastAppearOn, LinePtr temp ) { 
	if ( firstAppearOn == NULL ) { // �Ĥ@���i�� 
	  firstAppearOn = temp ;
	  lastAppearOn = temp;
	} // if 
	else { // �ĤG���H�� 
	  lastAppearOn -> next = temp ;
	  lastAppearOn = temp;
	} // else
} // InsertLine

void InsertToken( Str100 name, int kind, LinePtr temp1 ) {
	bool same = false ;
	TokenPtr find = new Token ;
	find = gFront ; // �䦳�S������ 
	while ( find && ! same ) {
	  if ( ! strcmp( find -> tokenStr, name ) ) {
	    find -> lastAppearOn -> next = temp1 ;
	    find -> lastAppearOn = temp1 ;
	  	same = true;
	  } // if
	
	  find = find -> next ;
	} // while
	if ( !same ) {
	  TokenPtr temp = new Token ;
	  temp -> next = NULL ;
	  temp -> tokenStr = new char[100] ;
	  temp -> firstAppearOn = NULL;
	  temp -> lastAppearOn = NULL;
	  strcpy( temp -> tokenStr, name );
	  if ( kind == 1 )
	    temp -> type  = IDENTIFIER;
  	  else if ( kind == 2 )
	    temp -> type = CONSTANT ;
	  else 
	    temp -> type = SPECIAL ;
	  if ( gFront == NULL ) {
	    gFront = temp ;
	    gRear = temp ;
	  } // if 
	  else {
	    gRear -> next = temp ;
	    gRear = temp;
	  } // else
	  
      InsertLine( temp -> firstAppearOn, temp -> lastAppearOn, temp1 ) ;
	} // if
	  
} // InsertToken


bool End( TokenPtr head ) {
	TokenPtr walk ;
	walk = head ; 

	
	while( walk ) {
	  if ( ! strcmp ( walk -> tokenStr, "END_OF_FILE" ) ) 
	    return true;
	  walk = walk -> next ;
	} // while
	
	return false;
} // CheckIfEnd


void PrintAll( TokenPtr head ) {
	LinePtr temp ;
	ColumnPtr temp2 ;
	TokenPtr walk = head;
	while( walk ) {
	  printf( "%s ", walk->tokenStr ); // token name
	  temp = walk -> firstAppearOn;
	  
	  while( temp ) {
	  	  temp2 = temp->firstAppearAt;
          printf( "(%d,%d)", temp->line, temp2->column );
		  temp = temp -> next;
		  
		  
	  } // while
	  printf( "\n" );
	  walk = walk -> next ;
	  
	} // while
	
} // PrintALL

int Order1 ( TokenPtr head ) { // �`�@���X��TOKEN
   TokenPtr walk;
   int num = 0;
   walk = head ;
   while( walk ) {
      num++;
      walk = walk -> next ;
   } // while
   
   return num;
} // Order1

void Order2 ( TokenPtr head, int & case1num, int & case2num, int & case3num ) { // 3��CASE�U���h��TOKEN
  case1num = 0;
  case2num = 0;
  case3num = 0;
  TokenPtr walk ;
  walk = head ;
  while ( walk ) {
  	if ( walk -> type == IDENTIFIER )
  	  case1num++;
  	if ( walk -> type == CONSTANT )
  	  case2num++;
  	if ( walk -> type == SPECIAL )
  	  case3num++;
  	walk = walk -> next ;
  } // while
} // Order2


bool Order3 ( TokenPtr head, Str100 temp ) { // �L�X���wTOKEN����m
  TokenPtr tokenwalk ;
  LinePtr linewalk ;
  ColumnPtr columnwalk ;
  tokenwalk = head ;
  while( tokenwalk && strcmp ( tokenwalk -> tokenStr, temp ) ) {
    tokenwalk = tokenwalk -> next ;  // while ��X�n�D��token 
    if ( ! tokenwalk )
      return false;
  } // while 
  
  if ( tokenwalk ) 
  	linewalk = tokenwalk -> firstAppearOn;
  while ( linewalk ) {
  	columnwalk = linewalk -> firstAppearAt;
  	printf( "(%d,%d)", linewalk -> line, columnwalk -> column );
	linewalk = linewalk -> next ;
  } // while
  
  return true;
} // order 3 



void Order4( TokenPtr head, int num ) {
	TokenPtr twalk ;
	LinePtr lwalk ;
	ColumnPtr cwalk ;
	twalk = head ;
	while ( twalk ) {
	  lwalk = twalk -> firstAppearOn;
	  while ( lwalk ) {
	    if ( num == lwalk -> line ) 
	      printf( "%s\n", twalk -> tokenStr ); 
	    lwalk = lwalk -> next ;
	  } // while
	  
	  twalk = twalk -> next ;
	} // while
	
} // Order4


void DeleteEOF( TokenPtr head, TokenPtr tail ) {
	TokenPtr walk ;
	walk = head ;
    while ( walk -> next != gRear ) 
    	walk = walk -> next ;
    // printf("TEST:%s   %s", walk -> tokenStr, gRear -> tokenStr );
	delete walk -> next ;
	walk -> next = NULL;
} // DeleteEOF

void Order( TokenPtr head ) { // by ASCII
	TokenPtr walk;
	TokenPtr walk2;
	Str100 temp;
	LinePtr ltemp;
	walk = head ;
    TokenType kind ;
	while( walk ) {
	  walk2 = walk -> next ;
	  while( walk2 ) {
    	if ( strcmp( walk -> tokenStr, walk2 -> tokenStr ) > 0 ) {	
    	  // token
	  	  strcpy( temp, walk -> tokenStr );
	  	  strcpy( walk -> tokenStr, walk2 -> tokenStr );
	  	  strcpy( walk2 -> tokenStr, temp );
	  	 
	  	  // first
	  	  ltemp = walk -> firstAppearOn ;
	  	  walk -> firstAppearOn = walk2 -> firstAppearOn ;
	  	  walk2 -> firstAppearOn = ltemp ;
	  	  // last
	  	  ltemp = walk -> lastAppearOn ;
	  	  walk -> lastAppearOn = walk2 -> lastAppearOn ;
	  	  walk2 -> lastAppearOn = ltemp ;
	  	  // type
	      kind = walk -> type;
	      walk -> type = walk2 -> type ;
	      walk2 -> type = kind;
	   
	    } // if  swap 
		
		walk2 = walk2 -> next ;
	  } // while
	
	  walk = walk -> next ;
	} // while 
	
} // Order

void Ordercolnum( TokenPtr head ) {
	TokenPtr temp ;
	temp = head ;
	LinePtr walk ;
	LinePtr walk2 ;
	int num ;
	
	walk = temp -> firstAppearOn;
	while ( temp ) {
	  walk = temp -> firstAppearOn;
	  while( walk ) {
		walk2 = walk -> next ;
		while ( walk2 ) {
			if ( walk -> line == walk2 -> line && walk -> firstAppearAt -> column > walk2 -> firstAppearAt -> column ) {
				num = walk -> firstAppearAt -> column ;
				walk -> firstAppearAt -> column  = walk2 -> firstAppearAt -> column ;
				walk2 -> firstAppearAt -> column = num ;
			} // if
			  
			walk2 = walk2 -> next ;  
		} // while
		
		walk = walk -> next ;
	  } // while
	  
	  temp = temp -> next ; 
	} // while
	
} // ordernum
int main() {
	char ch = '\0' ;
	bool end = false ;
	int order = 0;
	int num = 0;
	int order2case1num = 0;
	int order2case2num = 0;
	int order2case3num = 0;
	int whichline = 0 ;
	Str100 order3;

	while( ! End(gFront) ) {
		Getchar(ch);
     	Skipchar(ch);
		Assort(ch);
	} // while
	
	DeleteEOF( gFront, gRear );
	
	Order( gFront );
    Ordercolnum( gFront );
    
	PrintAll( gFront );
    printf( "\n�п�J���O�G\n" );
	printf( "1.�`�@���h�ֺ� token\n" );
	printf( "2.�T��case�U���h�� token\n" );
	printf( "3.�L�X���w token ����m (line number,column number) (�n�Ƨ�)\n" );
	printf( "4.�L�X�ҫ��w�����@ line �X�{���� token (�n�Ƨ�)\n" );
	printf( "5.����\n" );
	scanf("%d", &order ) ;
	
	while( order != 5 ) {
		if ( order == 1 ) {
			num = Order1(gFront);
			printf ( "\n�`�@%d��\n", num );
		} // if
		
		if ( order == 2 ) {	
			Order2(gFront, order2case1num, order2case2num, order2case3num );
			printf( "\nCase1 �@ %d��\n", order2case1num );
			printf( "Case2 �@ %d��\n", order2case2num );
			printf( "Case3 �@ %d��\n", order2case3num );
		} // if
		
		if ( order == 3 ) {
		  printf( "\n�п�J�n�j�M�� token : \n");
		  scanf( "%s", order3 );
         printf( "%s ", order3 );
		  if ( ! Order3( gFront, order3 ) ) 
		    printf( "�d�L��token : %s", order3 );
		  printf( "\n" );
		} // if
		
		if ( order == 4 ) {
		  printf( "\n�п�J�n���w�� line :\n" ) ;
		  scanf( "%d", &whichline );
		  Order4( gFront, whichline );
         printf( "\n" ) ;
		} // if 
		
		scanf( "%d", &order );
	} // while 
	
	printf ( "byebye" );
	
} // main
 