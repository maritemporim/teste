import java.util.Scanner;

class Atividade01{
    public static void main(String[] args){
    Scanner sc = new Scanner(System.in);

    int n;
    n = sc.nextInt();

    String[] c = new String[n];
    String[] branco = new String[n];
    String[] nome = new String[n];

    int pos = 0;
    int nega = 0;

    for(int i = 0; i < n; i++){
        c[i] = sc.next();

        if(c[i].equals("+")){ pos++; }
        else nega++;

        branco[i] = " ";
        nome[i] = sc.next();
    }

    System.out.println(n);
    

    for(int i = 0; i < n; i++){

        String[] novo = new String[n];
        novo[i] = Ordenar(n, nome[]);

        System.out.println(c[i] + " " + branco[i] + " " + nome[i]);
    }

    System.out.println("negativo: " + nega + " positivo:" + pos);
}

    public static void Ordenar(int n, String[] nome){
        for(int i = 0; i < n; n++){
            if(nome[i].compareTo(nome[i+1]) == 1){
                String tmp = nome[i];
                nome[i] = nome[i+1];
                nome[i+1] = tmp;

            }
        }
    }
}
