import java.util.Scanner;

public class LsTeste {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String palavra;
        palavra = sc.nextLine();

        while(isFim(palavra) == false){
            String vogal = isVogal(palavra);
            String consoante = isConsoante(palavra);
            String inteiro = isInteiro(palavra);
            String real = isReal(palavra); 

            System.out.println(vogal + " " + consoante + " " + inteiro + " " + real);
            palavra = sc.nextLine();
        }
        sc.close();
    }

    // Nessa parte o código pega a palavra e compara cada caractre com as respectivas vogais do alfabeto, se ele encontrar alguma letra que nao seja vogale el troca o resp para NÃO 

    public static String isVogal(String palavra){
        String resp = "SIM";
        for(int i = 0; i < palavra.length(); i++){
            char c = palavra.charAt(i);
            if(c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
               c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U'){
                resp = "NAO";
            }
        }
        return resp;
    }

    //Nesse trecho o código pega a palavra e ja assume que o resp é sim, logo depois pega ele pega a letra uma a uma da palavra, assim, como no codigo acima e cria uma variavel para vogais e letras no geral,, depois ele faz uma conferencia se o caracter atual é uma vogal e se ele é uma letra também, logo depois ele faz uma comparação onde se ve se ele não é uma letra ou tem alguma vogal, se esse if for true o resp muda para  não   

    public static String isConsoante(String palavra){
        String resp = "SIM";
        for(int i = 0; i < palavra.length(); i++){
            char c = palavra.charAt(i);
            boolean vogal = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
                               c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
            boolean letra = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');

            if(!letra || vogal){
                resp = "NAO";
            }
        }
        return resp;
    }


// nesse bloco vemos se o numero possui ponto ou virgula e se é compsoto apenas por digitos, se for verdadeiro ele possuir ponto e virgula e apenas numeros ele retorna sim e se não ele retorna nao
    public static String isReal(String palavra) {
        int pontos = 0;
        int digitos = 0;

        for (int i = 0; i < palavra.length(); i++) {
            char c = palavra.charAt(i);

            if (c == '.' || c == ',') {
                pontos++;
            } 
            else if (c >= '0' && c <= '9') {
                digitos++;
            } 
            else {
                return "NAO";
            }
        }

        if (digitos > 0 && pontos <= 1) {                  
            return "SIM";
        }

        return "NAO";
    }

    // se a palavra for diferente de zero ao nove comparando cada letra da string então o resp vira não 

    public static String isInteiro(String palavra){
        String resp = "SIM";
        for(int i = 0; i < palavra.length(); i++){
            if (palavra.charAt(i) < '0' || palavra.charAt(i) > '9') {
                resp = "NAO";
            }
        }
        return resp;
    }

    // compara se a palavra colocada é FIM

    public static boolean isFim(String palavra){
        boolean resp = false;
        if(palavra.length() == 3 && palavra.charAt(0) == 'F' && palavra.charAt(1) == 'I' && palavra.charAt(2) == 'M'){
            resp = true;
        }
        return resp;
    }
}