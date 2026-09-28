import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;
import java.util.Locale;

class Data {
    private int ano;
    private int mes;
    private int dia;

    public Data(int ano, int mes, int dia) {
        this.ano = ano;
        this.mes = mes;
        this.dia = dia;
    }

    public int getAno() { return ano; }
    public int getMes() { return mes; }
    public int getDia() { return dia; }

    // faz o parse de uma string no formato aaaa-mm-dd
    public static Data parseData(String s) {
        Data data = null;
        if (s != null && !s.isEmpty()) {
            String[] partes = s.split("-");
            if (partes.length == 3) {
                int ano = Integer.parseInt(partes[0].trim());
                int mes = Integer.parseInt(partes[1].trim());
                int dia = Integer.parseInt(partes[2].trim());
                data = new Data(ano, mes, dia);
            }
        }
        return data;
    }

    // retorna a data no formato dd/mm/yyyy conforme especificado
    public String format() {
        return String.format("%02d/%02d/%d", dia, mes, ano);
    }
}

class Veiculo {
    private int id;
    private String marca;
    private String modelo;
    private int ano;
    private String categoria;
    private String[] combustivel;
    private int cilindros;
    private double cilindrada;
    private String transmissao;
    private String tracao;
    private double consumoCidade;
    private double consumoEstrada;
    private double co2;
    private boolean turbo;
    private Data dataRegistro;

    public Veiculo(int id, String marca, String modelo, int ano, String categoria, String[] combustivel,
                   int cilindros, double cilindrada, String transmissao, String tracao,
                   double consumoCidade, double consumoEstrada, double co2, boolean turbo, Data dataRegistro) {
        this.id = id;
        this.marca = marca;
        this.modelo = modelo;
        this.ano = ano;
        this.categoria = categoria;
        this.combustivel = combustivel;
        this.cilindros = cilindros;
        this.cilindrada = cilindrada;
        this.transmissao = transmissao;
        this.tracao = tracao;
        this.consumoCidade = consumoCidade;
        this.consumoEstrada = consumoEstrada;
        this.co2 = co2;
        this.turbo = turbo;
        this.dataRegistro = dataRegistro;
    }

    // getters
    public int getId() { return id; }
    public String getMarca() { return marca; }
    public String getModelo() { return modelo; }
    public int getAno() { return ano; }
    public String getCategoria() { return categoria; }
    public String[] getCombustivel() { return combustivel; }
    public int getCilindros() { return cilindros; }
    public double getCilindrada() { return cilindrada; }
    public String getTransmissao() { return transmissao; }
    public String getTracao() { return tracao; }
    public double getConsumoCidade() { return consumoCidade; }
    public double getConsumoEstrada() { return consumoEstrada; }
    public double getCo2() { return co2; }
    public boolean isTurbo() { return turbo; }
    public Data getDataRegistro() { return dataRegistro; }

    // metodo para fazer o parse de uma linha do csv para um objeto veiculo
    public static Veiculo parseVeiculo(String s) {
        if (s == null || s.isEmpty()) return null;
        String[] campos = s.split(","); 
        
        int id = Integer.parseInt(campos[0].trim());
        String marca = campos[1].trim();
        String modelo = campos[2].trim();
        int ano = Integer.parseInt(campos[3].trim());
        String categoria = campos[4].trim();
        
        String[] combustivel = campos[5].trim().split(";");
        for(int i = 0; i < combustivel.length; i++) {
            combustivel[i] = combustivel[i].trim();
        }
        
        int cilindros = Integer.parseInt(campos[6].trim());
        double cilindrada = Double.parseDouble(campos[7].trim());
        String transmissao = campos[8].trim();
        String tracao = campos[9].trim();
        double consumoCidade = Double.parseDouble(campos[10].trim());
        double consumoEstrada = Double.parseDouble(campos[11].trim());
        double co2 = Double.parseDouble(campos[12].trim());
        boolean turbo = Boolean.parseBoolean(campos[13].trim());
        Data dataRegistro = Data.parseData(campos[14].trim());

        return new Veiculo(id, marca, modelo, ano, categoria, combustivel, cilindros, 
                           cilindrada, transmissao, tracao, consumoCidade, consumoEstrada, 
                           co2, turbo, dataRegistro);
    }

    // format usando locale.us para garantir o ponto nos decimais
    public String format() {
        StringBuilder sbComb = new StringBuilder();
        for (int i = 0; i < combustivel.length; i++) {
            sbComb.append(combustivel[i]);
            if (i < combustivel.length - 1) {
                sbComb.append(","); 
            }
        }

        String dataStr = (dataRegistro != null) ? dataRegistro.format() : "";

        return String.format(Locale.US, "[%d ## %s ## %s ## %d ## %s ## [%s] ## %d ## %.1f ## %s ## %s ## %.2f ## %.2f ## %.1f ## %b ## %s]",
                id, marca, modelo, ano, categoria, sbComb.toString(), cilindros,
                cilindrada, transmissao, tracao, consumoCidade, consumoEstrada,
                co2, turbo, dataStr);
    }
}

class LeitorCsv {
    // le o arquivo csv contando as linhas primeiro para criar o vetor com tamanho exato
    public static Veiculo[] ler(String caminhoArquivo) {
        int totalLinhas = 0;
        
        try (BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo))) {
            String linha = br.readLine(); // pula o cabecalho
            while ((linha = br.readLine()) != null) {
                if (!linha.trim().isEmpty()) {
                    totalLinhas++;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        Veiculo[] veiculos = new Veiculo[totalLinhas];
        int idx = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo))) {
            String linha = br.readLine(); // pula o cabecalho
            while ((linha = br.readLine()) != null) {
                if (!linha.trim().isEmpty()) {
                    Veiculo v = Veiculo.parseVeiculo(linha);
                    if (v != null) {
                        veiculos[idx++] = v;
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        return veiculos;
    }
}

public class Tp04 {
    public static void main(String[] args) {
        Veiculo[] todosVeiculos = LeitorCsv.ler("/tmp/veiculos.csv");
        
        Scanner scanner = new Scanner(System.in);
        
        // vetor dinamico manual para guardar os veiculos selecionados
        Veiculo[] selecionados = new Veiculo[10];
        int qtdSelecionados = 0;
        
        // leitura dos ids na entrada padrao ate encontrar -1
        while (scanner.hasNextInt()) {
            int idBusca = scanner.nextInt();
            if (idBusca == -1) {
                break;
            }
            
            for (int i = 0; i < todosVeiculos.length; i++) {
                if (todosVeiculos[i].getId() == idBusca) {
                    if (qtdSelecionados >= selecionados.length) {
                        Veiculo[] novo = new Veiculo[selecionados.length * 2];
                        for (int j = 0; j < qtdSelecionados; j++) {
                            novo[j] = selecionados[j];
                        }
                        selecionados = novo;
                    }
                    selecionados[qtdSelecionados++] = todosVeiculos[i];
                    break;
                }
            }
        }
        scanner.close();
        
        // ordenacao por insercao
        for (int i = 1; i < qtdSelecionados; i++) {
            Veiculo tmp = selecionados[i];
            int j = i - 1;
            
            while ((j >= 0) && (selecionados[j].getMarca().compareTo(tmp.getMarca()) > 0)) {
                selecionados[j + 1] = selecionados[j];
                j--;
            }
            selecionados[j + 1] = tmp;
        }
        
        // imprime os registros ordenados na saida padrao[cite: 2]
        for (int i = 0; i < qtdSelecionados; i++) {
            System.out.println(selecionados[i].format());
        }
    }
}