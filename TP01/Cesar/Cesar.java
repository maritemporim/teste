import java.util.*;

// o programa pega uma palavra e roda o loop até não possuir mais entradas, dentro do while
// ele cria uma string nova vazia, logo depois se cria uma avriavel onde pega cada letra da
// string plavra e acrescenta mais 3 e ao memso tempo vai adicionanado na string nova, depois
// se printa a string nova com a cifra de cesar 

class Cesar {
    public static void main(String[] args){
        Scanner sc = new Scanner(System.in);
        String palavra = sc.nextLine();

        while(isFim(palavra) == false){
            String nova = "";

            for(int i = 0; i < palavra.length(); i++){
                char jk = palavra.charAt(i);
                nova += (char) (jk + 3);
            }
            System.out.println(nova);
            palavra = sc.nextLine();
        }
        sc.close();
    }

    public static boolean isFim(String palavra){
        boolean resp = false;
        if(palavra.length() == 3 && palavra.charAt(0) == 'F' && palavra.charAt(1) == 'I' && palavra.charAt(2) == 'M'){
            resp = true;
        }
        return resp;
    }
}

