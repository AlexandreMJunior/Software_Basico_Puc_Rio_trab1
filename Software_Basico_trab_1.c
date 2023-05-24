// Alexandre Moreira de Moraes Junior - 2110520 - 3WA
// Thierno Alexander Dia - - 3WA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_BITS 128
typedef unsigned char BigInt[NUM_BITS / 8];

/* Atribuição (com extensão) */

void big_val(BigInt res, long val) {
  for (int i = 0; i < sizeof(long); i++) {
    res[i] = (val >> (i * 8)) & 0xFF;
  }

  if ((res[7] & 1) == 0) {
    for (int i = 8; i < 16; i++)
      res[i] = 0x00;
  }

  else if ((res[7] & 1) == 1) {
    for (int i = 8; i < 16; i++)
      res[i] = 0xFF;
  }
}

/* Operações Aritméticas */

void big_sum(BigInt res, BigInt a, BigInt b) {
  int resto = 0;
  int soma;
  for (int i = 0; i < 16; i++) {
    soma = a[i] + b[i] + resto;
    res[i] = soma & 0xFF;
    resto = soma >> 8;
  }
}

void big_comp2(BigInt res, BigInt a) {
  BigInt aux;
  BigInt um;
  big_val(um, 1);
  for (int i = 0; i < sizeof(BigInt); i++) {
    aux[i] = ~a[i];
  }
  big_sum(res, aux, um);
}

void big_sub(BigInt res, BigInt a, BigInt b) {
  BigInt aux;
  big_comp2(aux, b);
  big_sum(res, aux, a);
}

void big_mult(BigInt res, BigInt a, BigInt b) {
  int sign = 1;
  if (a[16 - 1] & 0x80) {
    big_comp2(a, a);
    sign = -sign;
  }
  if (b[16 - 1] & 0x80) {
    big_comp2(b, b);
    sign = -sign;
  }
  unsigned short vai_um = 0;
  unsigned int soma = 0;
  for (int i = 0; i < sizeof(BigInt); i++) {
    res[i] = 0;
  }
  for (int i = 0; i < sizeof(BigInt); i++) {
    vai_um = 0;
    for (int j = 0; j < sizeof(BigInt); j++) {
      soma = a[i] * b[j] + vai_um;
      res[i + j] += soma;
      vai_um = soma >> 8;
    }
  }
  if (sign == -1) {
    big_comp2(res, res);
  }
}

void big_shl(BigInt res, BigInt a, int n) {
  int qtd_bytes = n / 8;  // Número de bytes para deslocar
  int resto_bits = n % 8; // Número de bits para deslocar
  int carrega = 0;        // Variável do vai-um

  // Realiza o deslocamento dos bytes inteiros
  for (int i = 15 - qtd_bytes; i >= 0; i--) {
    res[i + qtd_bytes] = a[i];
  }

  // Realiza o deslocamento dos bits dentro do último byte
  if (resto_bits != 0) {
    for (int i = 15 - qtd_bytes; i >= 0; i--) {
      int prox_byte = i + qtd_bytes + 1;
      res[prox_byte] |= (a[i] >> (8 - resto_bits));
      res[i + qtd_bytes] = (a[i] << resto_bits) & 0xff;
    }
  }

  // Preenche com zeros os bytes que foram deslocados
  for (int i = 0; i < qtd_bytes; i++) {
    res[i] = 0;
  }
}


void big_shr(BigInt res, BigInt a, int n) {
    int qtd_bytes = n / 8;  // Número de bytes para deslocar
    int resto_bits = n % 8; // Número de bits para deslocar
    unsigned char carry = 0x00; // Carry variable

    for (int i = 15; i >= qtd_bytes; i--) {
        res[i - qtd_bytes] = a[i]; // Shift full bytes to the right
    }

    // Handle the remaining bits
    for (int i = 15; i >= qtd_bytes; i--) {
        unsigned char temp = a[i];
        temp >>= resto_bits;
        temp |= carry;
        carry = a[i] << (8 - resto_bits);
        res[i - qtd_bytes] |= temp;
    }

    // Clear the high bytes
    for (int i = 0; i < qtd_bytes; i++) {
        res[15 - i] = 0x00;
    }
}


void big_sar(BigInt res, BigInt a, int n) {
  int qtd_bytes = n / 8;   // Number of bytes to shift
  int resto_bits = n % 8;  // Number of bits to shift
  unsigned char carry = 0; // Carry variable

  // Shift complete bytes to the right
  for (int i = 0; i < 16 - qtd_bytes; i++) {
    res[i + qtd_bytes] = a[i];
  }

  // Shift remaining bits to the right
  for (int i = 16 - qtd_bytes - 1; i >= 0; i--) {
    unsigned char temp = a[i + qtd_bytes];
    temp >>= resto_bits;
    res[i + qtd_bytes] |= carry;
    carry = a[i + qtd_bytes] << (8 - resto_bits);
    res[i] = temp;
  }

  // Handle negative numbers
  if ((a[15] & 0xFF) == 0xFF) {
    for (int i = 16 - qtd_bytes; i < 16; i++) {
      res[i] = 0xFF;
    }
  } else {
    for (int i = 16 - qtd_bytes; i < 16; i++) {
      res[i] = 0x00;
    }
  }
}

int main(void) {
  BigInt vet;
  BigInt vet2;
  BigInt vet3;
  BigInt vet4;
  BigInt vet5;
  BigInt vet6;
  long value = 4;
  long value2 = -3;
  long value3 = -2;
  long value4 = -1;
  long value5 = 0;
  long value6 = -1024;

  printf("Vetor 1:\n");
  big_val(vet, value);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)vet[i]);
  }
  printf("\n");

  printf("\nVetor 2: \n");
  big_val(vet2, value2);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)vet2[i]);
  }

  printf("\n");
  printf("\nVetor 3: \n");
  big_val(vet3, value3);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)vet3[i]);
  }

  printf("\n");
  printf("\nVetor 4: \n");
  big_val(vet4, value4);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)vet4[i]);
  }

  printf("\n");
  printf("\nVetor 5: \n");
  big_val(vet5, value5);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)vet5[i]);
  }

  printf("\n");
  printf("\nVetor 6: \n");
  big_val(vet6, value6);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)vet6[i]);
  }

  // Soma
  BigInt res1;
  printf("\n\n");
  big_sum(res1, vet2, vet3);
  printf("Soma de vet 2 e vet 3: \n");
  for (int i = 0; i <= 15; i++) {
    printf("%02X ", (unsigned char)res1[i]);
  }

  // complemento a 2
  printf("\n");
  BigInt comp2;
  printf("\nComplemento a 2:\n");
  big_comp2(comp2, vet2);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)comp2[i]);
  }

  // subtracao
  printf("\n");
  BigInt sub;
  printf("\nSubtracao de vet 3 e vet 2:\n");
  big_sub(sub, vet3, vet2);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)sub[i]);
  }

  // multiplicacao
  printf("\n");
  BigInt mul;
  printf("\nMultiplicacao de vet 3 e vet 2:\n");
  big_mult(mul, vet3, vet2);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)mul[i]);
  }

  // shitf pra esquerda
  printf("\n");
  int n = 11;
  BigInt res2;
  printf("\nShiftando a esquerda o vetor 5 %d vezes:\n", n);
  big_shl(res2, vet5, n);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)res2[i]);
  }

  
  // shift pra direita
  printf("\n");
  int n4 = 4;
  BigInt res4;
  printf("\nShiftando a direita o vetor 4 %d vezes:\n", n4);
  big_shr(res4, vet4, n4);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)res4[i]);
  }
  

  // shitf aritmético
  printf("\n");
  int n2 = 4;
  BigInt res3;
  printf("\nShift aritmético do vetor 6 %d vezes:\n", n2);
  big_sar(res3, vet6, n2);
  for (int i = 0; i < sizeof(BigInt); i++) {
    printf("%02X ", (unsigned char)res3[i]);
  }
  return 0;
}

