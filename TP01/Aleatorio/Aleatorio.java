import java.util.Random;
import java.util.Scanner;

public class Aleatorio {
    Scanner sc = new Scanner(System.in);

    Random gerador = new Random();
    gerador.setSeed(4) ;
    System.out.println((char) ('a' + (Math.abs (gerador.nextInt ()) % 26)));

    String teste;


}
