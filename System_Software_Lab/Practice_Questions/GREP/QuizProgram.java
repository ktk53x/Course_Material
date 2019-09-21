/* A random java programme */
/* This program asks the user a bunch of random questions from the users. */

import java.util.Scanner;


public class QuizProgram {

     //Initializaions and Instantiations
    static Scanner cin = new Scanner(System.in);

    public static void main(String[] args)
    {
           double score = 0;
           final double NumberofQuestions = 5;

          System.out.println("\n\n\n\n\n\n\n\t\tQuiz 1.0\n\n");

           //Store questions and answers in 2 dimensional array
           String[][] QandA = {
                               {"Who sing this the song 'Locked Away':?","\nChoose 1 for R. City & Adam Levine's. \nChoose 2 for Justin Bieber.  \nChoose 3 for Selena Gomez. \nChoose 4 for Katy Perry.","1"},
                               {"Who sing this the song 'Beauty and the beast'?","\nChoose 1 for R. City & Adam Levine's. \nChoose 2 for Justin Bieber.  \nChoose 3 for Selena Gomez. \nChoose 4 for Katy Perry.","2"},
                               {"Who sing this the song 'Heal the world'?","\nChoose 1 for R. City & Adam Levine's. \nChoose 2 for Micheal Jackson.  \nChoose 3 for Selena Gomez. \nChoose 4 for Katy Perry.","2"},
                               {"Do you swear allegiance to the republic of Steve Jobs?","\nChoose 1 for Y. \nChoose 2 for N.","2"},
                               {"Are you publicly embarrassed when you burp '?","\nChoose 1 for Y. \nChoose 2 for N.","2"} };

          String[] Answers = new String[(int) NumberofQuestions];

         //loop through each string in the array and compare it to answers
         //for(int x = 0; x < NumberofQuestions; x++)
          int x=0;
          do
         {
              System.out.print("" + (x+1) + ". " + QandA[x][0] + "   "+QandA[x][1]);

              Answers[x] = String.valueOf(cin.nextInt());

              Answers[x].toLowerCase();

              if(QandA[x][2].equals(Answers[x]))
              {
                   score++;
              }//close if 
              else
              {
                  System.out.println("\n Incorrect. The right answer is "+QandA[x][2]);
              }

              System.out.print("\n");
              x++;
         }while(x<NumberofQuestions);//close outer loop                                 

         System.out.println("\n\t\tYou got " + score + " of "
                            + NumberofQuestions + " right!\n\n\n"); 

         System.out.println("\n\t\tYour percentage is " + ((score/NumberofQuestions)*100)+"%"); 


         System.exit(0);

    }//close main() function  

//-------------------------------------------------------------------------------

}//close Quiz clas

/* The end! */
