#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>


using namespace std;

FILE *fp;
int t;
string chipher_text;
string my_dictionary1;
string my_plaintext;
string output;
string my_substring_list[40];
string my_chipersub_list[40];
string my_dictionary2[200];
string possible_wordlist[200][20]={""};
int my_dictionary2_pair_indexes[10000][200]={-1};

int chipher_index=0;
int my_substring_count;
int current_dict_index=0;
int my_chiper_lenght=0;
int myciplen=0;
int my_dict1_length = 0;
int diff_list[150][40]={0};//This is array will contain differences
int row=0;
int column=0;
int found = 0;

void read_dictonary2();
string find_plaintext(string);

// Code to read the dictionary file
string get_file_contents(const char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if(fp==NULL)
    cout << "check the dictionay.txt file place it in the CWR\n" << endl;
  if (fp)
  {
    string contents;
    fseek(fp, 0, SEEK_END);
    contents.resize(ftell(fp));
    rewind(fp);
    fread(&contents[0], 1, contents.size(), fp);
    fclose(fp);
    return(contents);
  }
  throw(errno);
}


// Start of Main function
int main()
{
    printf("************Start****************\n");
    printf("Enter the length of the key: \n");
    scanf("%d",&t);
    printf("Length of the key (t)=%d\n",t);
    printf("Enter the cipher text: ");
    cin>>chipher_text;
    my_chiper_lenght=chipher_text.length();
    myciplen=chipher_text.length();
    cout << "Cipher Text is:"<<chipher_text<< endl;
    my_dictionary1 = get_file_contents("Dictionary1.txt");
    my_dict1_length = my_dictionary1.length();


    // Search for the plaintext from the Dictionary1
    for( current_dict_index=0; current_dict_index < my_dict1_length-myciplen+1; current_dict_index++)
    {
         
        my_plaintext.clear();
        my_chiper_lenght=chipher_text.length();
        my_plaintext = my_dictionary1.substr(current_dict_index,my_chiper_lenght);
        //cout<< my_plaintext<<endl;
        //system("PAUSE");
         
        output = find_plaintext(my_plaintext);
        if (found == 1)
        {
           cout<<"PlainText is: "<<output<<endl;
           break;
        }
                    
    } //end outer for loop for dictionary1
    
    
    //while(1);
    
    if(found == 0)
    {
      // If plaintext couldn't found from Dict1, try searching in Dict2.
      cout<<"Not found in Dictionary1."<<endl;
      system("PAUSE");
      read_dictonary2();
      
    }
    
    printf("************END****************\n");
    system("PAUSE");
    return 0;
    
}//end of main()




// Logic to find out the plain_text from the input text.
string find_plaintext(string str1)
{
       ///compute the substring count.
        if(chipher_text.length()%t==0)
            my_substring_count=chipher_text.length()/t;
        else
            my_substring_count=(int)chipher_text.length()/t+1;
    
        //cout << "length of chipher="<<chipher_text.length()<< endl;
        //cout << "substring_count="<<my_substring_count<< endl;         
        
        
        ///now that we know the substring count get the substrings from dictionary
        //cout<<"---------substrings from dictionary---------"<<endl;
        int current_pltxt_index = 0;
        my_chiper_lenght = chipher_text.length();
        for(int i=0; i<my_substring_count; i++)
        {
            if(my_chiper_lenght<t)
             my_substring_list[i] = str1.substr(current_pltxt_index,my_chiper_lenght);
            else
             my_substring_list[i] = str1.substr(current_pltxt_index,t);
             
            current_pltxt_index+=t;
            //cout << i <<" substr =" << my_substring_list[i]<<endl;
            my_chiper_lenght-=t;
        }
        
        ///now get the cipher substrings from dictionary
        //cout<<"---------substrings from cipher text---------"<<endl;
        chipher_index = 0;
        my_chiper_lenght=chipher_text.length();        
        for(int  i=0; i<my_substring_count; i++)
        {
             if(my_chiper_lenght < t)
                 my_chipersub_list[i] = chipher_text.substr(chipher_index,my_chiper_lenght);
             else
                 my_chipersub_list[i] = chipher_text.substr(chipher_index,t);
                 
            chipher_index+=t;
            //cout<<i<<" substr ="<<my_chipersub_list[i]<<endl;
            my_chiper_lenght-=t;
        }
        //cout << "t=" << t << endl;        
        
        ///now that we have both the substrings from dictionary and cipher text
        ///we will compute the difference of ascii values of each character        
        // Build the difference array to compare
        row=0; column=0;
        for(int i=0; i<my_substring_count; i++)
        {
            //cout<<i<<" d substr ="<<my_chipersub_list[i]<<endl;
            //cout<<i<<" c substr ="<<my_substring_list[i]<<endl;
            for(int j=0; j<my_substring_list[i].length(); j++)
            {
                diff_list[row][column] = abs( my_chipersub_list[i].at(j)- my_substring_list[i].at(j) );
                column++;
            }
    
            row++;
            column=0;
        }        


        ///print the matrix of differences
        /*
        for(int x=0; x < my_substring_count; x++)  
        {
            for(int y=0; y<t; y++) 
            {
                cout << diff_list[x][y] << " ";  
            }
            cout<<endl;
        }     */


        ///now we have the difference list check if all the
        ///columns have the same numbers
        int is_diff=0;
        for(int x=0; x<t; x++)
        {
            if(is_diff == 1)
                break;
            for(int y=0; y<my_substring_count; y++)
            {
                if((diff_list[0][x]-diff_list[y][x] != 0) && (diff_list[0][x]-diff_list[y][x] != diff_list[0][x]))
                {
                    is_diff=1;
                    break;
                }
            }
            //cout<<endl;  // when the inner loop is done, go to a new line
        }
        
        if(is_diff==0)
        {
            found = 1;
            return str1.substr(0,chipher_text.length());
            //cout<<"PlainText is: "<<my_plaintext.substr(0,chipher_text.length())<<endl;
            //break;
        }
        else
        {
            found = 0;
            return "";
        }
        
} //End of find_plaintext()







void read_dictonary2()
{
    int first_index=0, next_index = 1, last_index=1;
    int current_length=0;
    int pwl_index = 0;
    int i=0;
    int no_of_words = 0;
    string filename="Dictionary2.txt";
    string current_string;
    string line;
    ifstream file;

    file.open(filename.c_str());
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file,line); 
            my_dictionary2[i]=line; 
            i++;
        }
        no_of_words = i;
        file.close();
    }
    else
    {
        printf("Cannot open Dictionary2\n");
    }
    
    //display the file content
    /*
    for(int p=0; p<i; p++)
    {
            cout << my_dictionary2[p]<<endl;
    } */
    
    //system("PAUSE");
    //iterate the for loop
    do_again:

        current_string=my_dictionary2[first_index];
        current_length=my_dictionary2[first_index].length();

        while(last_index<no_of_words)
        {
            int next_length = my_dictionary2[next_index].length();
            int last_length = my_dictionary2[last_index].length();
            
            if(current_length+next_length < myciplen)
            {
                current_string+=my_dictionary2[next_index];
                current_length = current_length + next_length;
                next_index++;
            }
            else if(current_length+next_length == myciplen)
            {
                current_string+=my_dictionary2[next_index];
                output = find_plaintext(current_string);
                if (found == 1)
                {
                   cout<<"PlainText is: "<<output<<endl;
                   break;
                }
                else
                {
                    last_index++;
                    next_index = last_index;
                    current_string.clear();
                    current_string = my_dictionary2[first_index];
                    current_length = my_dictionary2[first_index].length();
                }
            }
            else
            {
                last_index++;
                next_index = last_index;
                current_string.clear();
                current_string = my_dictionary2[first_index];
                current_length = my_dictionary2[first_index].length();
            }
            
            
            
            if(last_index==no_of_words-1)
            {
                first_index++;
                next_index=first_index+1;
                last_index=first_index+1;
                current_length=0;
                current_string.clear();
                goto do_again;
            }
        }//end of while(loop)
        
        if(found == 0)
        {
          cout<<"PlainText couldn't found in Dictionary1 or Dictionary2."<<endl<<endl;
        }
        
}//end of read_dictonary
