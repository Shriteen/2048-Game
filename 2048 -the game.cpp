#include<iostream>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
using namespace std;
class board
{
	protected:
		int grid[6][6],size;
		void shift_up();
		void shift_left();
		void shift_down();
		void shift_right();
		bool can_play_more();
		inline int random_i();
		inline int random_v();
		void fresh_num();
};
class player
{
	protected:
		char name[20];
		int score;
		void ask_name();
		void _2048();
};
class game:private board,player
{
	private:
		void merge_up();
		void merge_left();
		void merge_down();
		void merge_right();
		void display();
		void over();
		bool pause();
		bool save_game();
		void load_hs(int);
	public:
		game(int);
		game(){}
		void gameplay();
		void set_data(int s,int arr[][6],char n[],int sc);
		void show_data();
};
class menu
{
	private:
		void new_game();
		void load_game();
		void setup();
		void high_scores();
		void help();
	public:
		void master_menu();
};
struct record
{
	game g;
	char t[24];
	int flag;
};
struct hiscore
{
	char at[24],player[20];
	int hsc;
}ghs_rec;
game::game(int s)
{
	size=s;
	for(int i=0;i<size;i++) for(int j=0;j<size;j++) grid[i][j]=0;
	score=0;
	srand(time(NULL));
	ask_name();
	grid[random_i()][random_i()]=random_v();
}
void game::gameplay()
{
	load_hs(size);
	char dpad; bool flag=1;
	
	while(can_play_more()&&flag)
	{
		
		display();
		do cin>>dpad; while(dpad!='w'&&dpad!='a'&&dpad!='s'&&dpad!='d'&&dpad!='p');
		switch(dpad)
		{
			case 'w': shift_up(); merge_up(); shift_up(); break;
			case 'a': shift_left(); merge_left(); shift_left(); break;
			case 's': shift_down(); merge_down(); shift_down();break;
			case 'd': shift_right(); merge_right(); shift_right(); break;
			case 'p': flag=pause(); break;
			default:;
		}
		if(dpad!='p') fresh_num();
	}
	if(flag) over();
	
}
void menu::master_menu()
{
	setup();
	int opt;
	do
	{
		
		system("clear");
		cout<<"\n\tEnter your choice\n\n\t\t1:New Game\n\t\t2:Continue Saved Game\n\t\t3:High Scores\n\t\t4:Help\n\t\t5:Exit\n";
		do cin>>opt; while(!(opt>0&&opt<6));
		switch(opt)
		{
			case 1: new_game(); break;
			case 2: load_game(); break;
			case 3: high_scores(); break;
			case 4: help(); break;
			case 5: break;
		}
	}while(opt!=5);
	
}
void player::ask_name()
{
	system("clear");
	cout<<"Enter player name\n";
	cin.ignore();
	cin>>name;
	//gets(name);
}
void game::display()
{
	int x,y;
	x=14-size;
	y=10-size;
	system("clear");
	cout<<endl<<setfill(' ')<<setw(10)<<"Player:"<<name<<setw(20)<<"Score:"<<score<<setw(23)<<"High Score:"<<ghs_rec.hsc<<endl<<endl;
	cout<<setfill('_')<<setw(80)<<'\n';
	for(int i=0;i<size;i++)
	{	
		for(int k=1;k<=y;k++) cout<<endl;
		cout<<setfill(' ')<<setw(2.5*x)<<'\0';
		for(int j=0;j<size;j++)
			cout<<grid[i][j]<<setw(x);
	}
	cout<<endl<<endl<<endl<<endl<<endl<<setfill('_')<<setw(80)<<'\n';
}
inline int board::random_i()
{
	return rand()%size;
}
inline int board::random_v()
{
	if(random_i()==0) return 4;
	else return 2;
}
void board::shift_up()
{
	for(int i=0;i<size;i++) for(int j=0;j<size-1;) if(grid[j][i]==0&&grid[j+1][i]!=0)
	{
		grid[j][i]=grid[j+1][i];
		grid[j+1][i]=0;
		if(j>0) j--; else j++;
	}else j++;
}
void board::shift_left()
{
	for(int i=0;i<size;i++) for(int j=0;j<size-1;) if(grid[i][j]==0&&grid[i][j+1]!=0)
	{
		grid[i][j]=grid[i][j+1];
		grid[i][j+1]=0;
		if(j>0) j--; else j++;
	} else j++;
}
void board::shift_down()
{
	for(int i=0;i<size;i++) for(int j=size-1;j>0;) if(grid[j][i]==0&&grid[j-1][i]!=0)
	{
		grid[j][i]=grid[j-1][i];
		grid[j-1][i]=0;
		if(j<size-1) j++; else j--;
	} else j--;
}
void board::shift_right()
{
	for(int i=0;i<size;i++) for(int j=size-1;j>0;) if(grid[i][j]==0&&grid[i][j-1]!=0)
	{
		grid[i][j]=grid[i][j-1];
		grid[i][j-1]=0;
		if(j<size-1) j++; else j--;
	} else j--;
}
bool board::can_play_more()
{
	for(int i=0;i<size;i++) for(int j=0;j<size;j++) if(grid[i][j]==0) return 1;
	for(int i=0;i<size;i++) for(int j=0;j<size-1;j++) if(grid[i][j]==grid[i][j+1]||grid[j][i]==grid[j+1][i]) return 1;
	return 0;
}
void game::merge_up()
{
	for(int i=0;i<size;i++) for(int j=size-1;j>0;j--) if(grid[j][i]!=0&&grid[j][i]==grid[j-1][i])
	{
		grid[j-1][i]*=2; grid[j][i]=0; score+=grid[j-1][i]; 
		if(grid[j-1][i]==2048) _2048();
	}
}
void game::merge_left()
{
	for(int i=0;i<size;i++) for(int j=size-1;j>0;j--) if(grid[i][j]!=0&&grid[i][j]==grid[i][j-1])
	{
		grid[i][j-1]*=2; grid[i][j]=0; score+=grid[i][j-1];
		if(grid[i][j-1]==2048) _2048();
	}
}
void game::merge_down()
{
	for(int i=0;i<size-1;i++) for(int j=0;j<size;j++) if(grid[i][j]!=0&&grid[i][j]==grid[i+1][j])
	{
		grid[i+1][j]*=2; grid[i][j]=0; score+=grid[i+1][j];
		if(grid[i+1][j]==2048) _2048();
	}
}
void game::merge_right()
{
	for(int i=0;i<size;i++) for(int j=0;j<size-1;j++) if(grid[i][j]!=0&&grid[i][j]==grid[i][j+1])
	{
		grid[i][j+1]*=2; grid[i][j]=0; score+=grid[i][j+1];
		if(grid[i][j+1]==2048) _2048();
	}
}
void board::fresh_num()
{
	int x=random_i(),y=random_i();
	for(int i=0;i<size;i++) for(int j=0;j<size;j++)
	{
		if(grid[((x+i)%size)][((y+j)%size)]==0)
		{
			grid[((x+i)%size)][((y+j)%size)]=random_v();
			goto out;
		}
	}
	out:;
}
void game::over()
{
	
	display();
	cout<<endl<<setfill(' ')<<setw(45)<<"GAME OVER"<<endl;
	cout<<"\tscore:"<<score<<endl;
	if(score>ghs_rec.hsc)
	{
		fstream file;
		struct hiscore new_hsc;
		time_t now;
		time(&now); strcpy(new_hsc.at,asctime(localtime(&now))); strcpy(new_hsc.player,name); new_hsc.hsc=score;
		file.open("high_scores",ios::in|ios::out|ios::binary);
		file.seekp((size-3)*sizeof(new_hsc),ios::beg);
		file.write((char*)&new_hsc,sizeof(new_hsc));
		file.close();
		cout<<endl<<setfill(' ')<<setw(45)<<"NEW HIGHSCORE!"<<endl;
	}
	time_t start,stop;
	time(&start);
	do time(&stop); while(stop<start+4);
	
	char cont;
	cout<<"\n\nPress 0 to continue\n";
	do cin>>cont; while(cont!='0');
}
bool game::pause()
{
	int ch;
	system("clear");
	cout<<"\n\tEnter your choice\n\n\t\t1:Resume\n\t\t2:Restart\n\t\t3:Save and Exit\n\t\t4:Discard and Exit\n";
	do cin>>ch; while(!(ch>0&&ch<5));
	switch(ch)
	{
		case 1: return 1;
		case 2: for(int i=0;i<size;i++) for(int j=0;j<size;j++) grid[i][j]=0;
				score=0;
				srand(time(NULL));
				grid[random_i()][random_i()]=random_v();
				return 1;
		case 3: return save_game();
		case 4: return 0;
		default:;
	}
}
void menu::new_game()
{
	int b;
	system("clear");
	cout<<"\n\tEnter your choice\n\n\t\t1:3 by 3\n\t\t2:4 by 4\n\t\t3:5 by 5\n\t\t4:6 by 6\n\t\t5:Back\n";
	do cin>>b; while(!(b>0&&b<6));
	switch(b)
	{
		case 1: { game a(3); a.gameplay(); break; }
		case 2: { game a(4); a.gameplay(); break; }
		case 3: { game a(5); a.gameplay(); break; }
		case 4: { game a(6); a.gameplay(); break; }
		case 5: break;
	}
}
void game::set_data(int s,int arr[][6],char n[],int sc)
{
	size=s;
	for(int i=0;i<size;i++) for(int j=0;j<size;j++) grid[i][j]=arr[i][j];
	strcpy(name,n);
	score=sc;
}
void game::show_data()
{
	cout<<setfill(' ')<<setw(20)<<name<<setw(10)<<score<<setw(10)<<size<<"\t";
}
bool game::save_game()
{
	struct record b;
	int rec_no;
	fstream file;
	system("clear");
	file.open("saved_games",ios::out|ios::binary|ios::app); file.close();
	file.open("saved_games",ios::in|ios::out|ios::binary|ios::ate);
	rec_no=file.tellg()/200;
	time_t now;
	time(&now);
	if(rec_no<3)
	{
		b.g.set_data(size,grid,name,score);
		strcpy(b.t,asctime(localtime(&now)));
		b.flag=1;
		file.write((char*)&b,sizeof(b));
		file.close();
		cout<<"\n\tGame saved\n";
	}
	else
	{
		file.seekg(0,ios::beg);
		cout<<"\n\tSelect a slot\n\n";
		cout<<setfill(' ')<<setw(24)<<"Name"<<setw(10)<<"Score"<<setw(10)<<"size"<<"\t\t"<<"Saved on\n";
		for(int i=1;i<=3;i++)
		{
			file.read((char*)&b,sizeof(b));
			if(b.flag==0) cout<<i<<"Empty Record\n";
			else
			{
				cout<<i<<":";
				b.g.show_data();
				for(int j=0;j<24;j++) cout<<b.t[j];
				cout<<endl;
			}
		}
		cout<<"4:Back\n";
		int pos;
		do cin>>pos; while(pos<1&&pos>4);
		if(pos==4){ file.close(); return 1;}
		b.g.set_data(size,grid,name,score);
		strcpy(b.t,asctime(localtime(&now)));
		b.flag=1;
		file.seekp((pos-1)*200,ios::beg);
		file.write((char*)&b,sizeof(b));
		file.close();
		cout<<"\n\tGame saved\n";
	}
	char back;
	cout<<"\nPress 0 to continue\n";
	do cin>>back; while(back!='0');
	return 0;
}
void menu::load_game()
{
	struct record c;
	int rec_no;
	fstream file;
	system("clear");
	file.open("saved_games",ios::out|ios::binary|ios::app); file.close();
	file.open("saved_games",ios::in|ios::out|ios::binary|ios::ate);
	rec_no=file.tellg()/200;
	if(rec_no==0)
	{
		cout<<"\n\tNo saved games\n";
		file.close();
		char back;
		cout<<"\nPress 0 to return to main menu\n";
		do cin>>back; while(back!='0');
	}
	else
	{
		file.seekg(0,ios::beg);
		cout<<"\n\tSelect a save\n\n";
		cout<<setfill(' ')<<setw(24)<<"Name"<<setw(10)<<"Score"<<setw(10)<<"size"<<"\t\t"<<"Saved on\n";
		int i;
		for(i=1;i<=rec_no;i++)
		{
			file.read((char*)&c,sizeof(c));
			if(c.flag==0) cout<<i<<":Empty Record\n";
			else
			{
				cout<<i<<":";
				c.g.show_data();
				for(int j=0;j<24;j++) cout<<c.t[j];
				cout<<endl;
			}
		}
		cout<<i<<":Back\n";
		int pos;
		do cin>>pos; while(pos<1&&pos>rec_no+1);
		if(pos==i) { file.close(); return; }
		file.seekg((pos-1)*200,ios::beg);
		file.read((char*)&c,sizeof(c));
		struct record d;
		int temp[6][6]={{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
		d.flag=0;
		d.g.set_data(6,temp,"Easter_egg",0);
		file.seekp((pos-1)*200,ios::beg);
		file.write((char*)&d,sizeof(d));
		file.close();
		srand(time(NULL));
		c.g.gameplay();
	}
}
void menu::setup()
{
	fstream file;
	file.open("high_scores",ios::out|ios::binary|ios::app); file.close();
	file.open("high_scores",ios::out|ios::in|ios::binary|ios::ate);
	if(file.tellp()==0)
	{
		struct hiscore set;
		strcpy(set.at,"N/A                     "); strcpy(set.player,"N/A"); set.hsc=0;
		file.seekp(0,ios::beg);
		for(int i=0;i<4;i++) file.write((char*)&set,sizeof(set));
	}
	file.close();
}
void menu::high_scores()
{
	system("clear");
	
	cout<<"\n\tHigh Scores\n\n";
	struct hiscore flaunt;
	fstream file;
	file.open("high_scores",ios::in|ios::binary);
	cout<<setfill(' ')<<setw(10)<<"size"<<setw(24)<<"Name"<<setw(10)<<"Score"<<"\t\t"<<"Beaten on\n";
	for(int i=3;i<7;i++)
	{
		file.read((char*)&flaunt,sizeof(flaunt));
		cout<<setfill(' ')<<setw(9)<<i<<setw(24)<<flaunt.player<<setw(10)<<flaunt.hsc<<"\t\t";
		for(int j=0;j<24;j++) cout<<flaunt.at[j];
	}
	cout<<endl<<endl;
	file.close();
	char back;
	cout<<"Press 0 to return to main menu\n";
	do cin>>back; while(back!='0');
	
}
void game::load_hs(int sz)
{
	fstream file;
	file.open("high_scores",ios::in|ios::binary);
	file.seekg((sz-3)*sizeof(ghs_rec),ios::beg);
	file.read((char*)&ghs_rec,sizeof(ghs_rec));
	file.close();
}
void menu::help()
{
	system("clear");
	
	cout<<"\n\tHelp\n\n2048 is a game where you combine numbers to gain a higher number.\n\n";
	cout<<"The rules are simple. You move either up,down,left or right so that the numbers move in that direction until they hit the boundary or another number.\n";
	cout<<"If two similar numbers hit they merge to form double the value.\n";
	cout<<"Every time you merge a number your score increases by the value of new number\n";
	cout<<"You start with a single number. At every move a new number i.e. either 2 or 4 is added at a random place in board.\n";
	cout<<"The game continues till all the positions are filled and no merging is possible.\n\n";
	cout<<setfill('-')<<setw(80)<<"\n";
	cout<<"\nOriginal 2048 is 4 by 4 but this game allows to select 3 to 6 sizes.\n";
	cout<<"Original 2048 goal is to create 2048 but this version follows arcade style of establishing higher score.\n\n";
	cout<<setfill('-')<<setw(80)<<"\n";
	cout<<"\nControls:\nw to move up\na to move left\ns to move down\nd to move right\nPress enter to confirm the input.\n*Use lower case\n\n";
	cout<<"Press p in game to show options.\n";
	cout<<"Options are resume,restart,save and exit,discard and exit.\n";
	cout<<"Save and exit allows players to continue the game later.\n";
	cout<<"Discard and exit discards the game progress.\n";	
	cout<<"The saved game can be continued by choosing continue saved game from the main menu and selecting the save.\n\n";
	cout<<setfill('-')<<setw(80)<<"\n";
	cout<<"\nThe game saves high scores from each difficulty level along with the time and player name which is asked at beginning of every new game.\n";
	cout<<"These can be viewed in High scores from main menu.\n";
	cout<<"Deleting saved_games and high_score files will make you lose the saved games and high scores so be careful.\n\n";
	cout<<setfill(' ')<<setw(50)<<"Have fun! \1\n\n";
	cout<<setfill('-')<<setw(80)<<"\n";
	char back;
	cout<<"Press 0 to return to main menu\n";
	do cin>>back; while(back!='0');
	
}
void player::_2048()
{
	system("clear");
	
	cout<<setfill(' ')<<endl<<endl<<endl<<endl<<setw(44)<<"2048!!!!\n";
	time_t start,stop;
	time(&start); do time(&stop); while(stop<start+1); 
	time(&start); do time(&stop); while(stop<start+1); 
	time(&start); do time(&stop); while(stop<start+1); 
}
int main()
{
	menu m;
	m.master_menu();
}
