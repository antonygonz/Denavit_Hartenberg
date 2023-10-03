#include <stdio.h>//libreria para entrada y salida
#include <stdlib.h>
#include <string>//libreria para manipulacion de strings
#include <functional>
#include <iostream>
#include <cstdlib>
#include <cmath>//libreria para Sen, Cos y Tan

//using string_t = std::string;
#define PI 3.14159265
const float toRad = PI / 180;
const float toDegre = 180 / PI;
const int numCols = 4;
int numRows = 1;
float **matrixDenavit;
float ***matrixTransformacion;
int *variableVector;
float *VectorEntradas;
bool tablaGenerada = false;
bool vectorZero = true;
bool runing = true;
float matrixResultante[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
float* vectorCoordenadas;


void llenadoMatrix4x4() {
    for (int i = 0; i < numCols; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            if (i==j)
            {
                matrixResultante[i][j] = 1;
            }
            else {
                matrixResultante[i][j] = 0;
            }
        }
    }
}
bool isFloat(const std::string& str)
{
    bool result = true;
    try
    {
        // Convert string to float
        float n = std::stof(str);
    }
    catch (const std::invalid_argument& e)
    {
        result = false;
    }
    catch (const std::out_of_range& e)
    {
        result = false;
    }
    return result;
}
void denavitMatrix(){
    {
        matrixDenavit = (float**)malloc(numRows * sizeof(float*));
        variableVector = (int*)malloc(numRows * sizeof(int));
        VectorEntradas = (float*)malloc(numRows * sizeof(float));
        for (int i = 0; i < numRows; ++i) {
            matrixDenavit[i] = (float*)malloc(numCols * sizeof(float));
        }

        int userInput;
        std::string strInput = "";
        do {
            printf("Marque con una X la variable (solamente se permite 1 variable por eslabon, de tener mas de 1 se tomara como valor cero)\n");
            printf("Los valores para los angulos 0n y A0 se requieren en Grados\n");
            printf("Introduzca los valores de la tabla denavit-hartenberg para el eslabon %d siguiendo el siguiente orden:\n0n   dn   an   An\n",numRows);
            /*for (int col = 0; col < numCols; ++col) {
                //scanf_s("%f", &matrix[numRows - 1][col]);
                scanf_s("%s", strInput);
                matrix[numRows - 1][col] = strtof(strInput,NULL);
            }*/
            variableVector[numRows - 1] = 0;
            VectorEntradas[numRows - 1] = 0;
            for (int col = 0; col < numCols; ++col) {
                std::cin >> strInput;
                if (isFloat(strInput))
                {
                    matrixDenavit[numRows - 1][col] = std::stof(strInput);
                }
                else
                {
                    matrixDenavit[numRows - 1][col] = 0;
                    variableVector[numRows - 1] = col;
                }
            }

            std::cin.ignore(INT_MAX, '\n');
            std::cin.clear();

            printf("Deseas agregar otra fila? 1) Si, 0) No: ");
            scanf_s("%d", &userInput);
            printf("\n");
            if (userInput == 1) {
                numRows++;//valor que hace crecer la tabla
                float** temp = (float**)malloc(numRows * sizeof(float*));
                int* tempVector = (int*)malloc(numRows * sizeof(int));
                float* VectorEntradasTemp = (float*)malloc(numRows * sizeof(float));
                for (int i = 0; i < numRows - 1; ++i) {
                    temp[i] = matrixDenavit[i];
                    tempVector[i] = variableVector[i];
                    VectorEntradasTemp[i] = 0;
                }
                temp[numRows - 1] = (float*)malloc(numCols * sizeof(float));
                free(matrixDenavit);
                free(variableVector);
                free(VectorEntradas);
                variableVector = tempVector;
                matrixDenavit = temp;
                VectorEntradas = VectorEntradasTemp;
            }
        } while (userInput == 1);

    }

    tablaGenerada = true;
}
void printMatrixDenavit(){
    printf("Matriz Denavit-Hartenberg:\n");
    printf("    0n   dn   an   An\n");
    for (int row = 0; row < numRows; ++row) {
        printf("L%d ",row+1);
        for (int col = 0; col < numCols; ++col) {
            printf("%.2f ", matrixDenavit[row][col]);

        }
        printf("\n");
    }
}
void printVectorEntradas() {
    printf("\nVector de Entradas: \n");
    printf("El vector que indica el valor de la variable\n");//debug
    for (int row = 0; row < numRows; ++row) {
        printf("%f ", VectorEntradas[row]);
    }
    printf("\n\n");
}
void printVariableVector() {
    printf("Vector de variables: \n");
    printf("El vector que indica la posicion de la variable\n");//debug
    for (int row = 0; row < numRows; ++row) {
        printf("%d ", variableVector[row]);
    }
    printf("\n");
}
void sobreescribirTabla() {
    for (int i = 0; i < numRows; i++)//para cada columna de la tabla
    {
        matrixDenavit[i][variableVector[i]] = VectorEntradas[i];
    }
}
void cambioEntradas() {
    vectorZero = true;
    float entrada = 0;
    for (int i = 0; i < numRows; i++)
    {
        printf("Elija el valor de la variable ");
        if (variableVector[i] == 0)
        {
            printf("0%d\n",i+1);
        }
        else if (variableVector[i] == 1) {
            printf("d%d\n", i + 1);
        }
        else if (variableVector[i] == 2) {
            printf("a%d\n", i + 1);
        }
        else if (variableVector[i] == 3) {
            printf("A%d\n", i + 1);
        }
        else{
            printf("_%d\n", i + 1);
        }
        
        scanf_s("%f", &entrada);
        VectorEntradas[i] = entrada;
        if (vectorZero)
        {
            if (VectorEntradas[i]!=0)
            {
                vectorZero = false;
            }
        }
        entrada = 0;

    }
    sobreescribirTabla();
    printVectorEntradas();
}
void crearMatricesTransformacion() {
    //float matrices[numRows][numCols][numCols];
    matrixTransformacion = (float***)malloc(numRows * sizeof(float**));
    for (int i = 0; i < numRows; ++i) {
        matrixTransformacion[i] = (float**)malloc(numCols * sizeof(float*));
        for (int j = 0; j < numCols; j++)
        {
            matrixTransformacion[i][j] = (float*)malloc(numCols * sizeof(float));
            for (int k = 0; k < numCols; k++)
            {
                if (j==k)
                {
                    matrixTransformacion[i][j][k] = 1.0;
                }
                else {
                    matrixTransformacion[i][j][k] = 0.0;
                }
                
            }
        }
    }
}
void llenarMatricesTransformacion() {
    /*
        {Cos0n   -Sen0nCosAn    Sen0nSenAn    anCos0n}
        {Sen0n    Cos0nCosAn   -Cos0nSenAn    anSen0n}
        {zero       SenAn         CosAn          dn  }
        {zero       zero           zero          1   }
        */
    for (int i = 0; i < numRows; i++)
    {
        //siguiente calculo de Cos0n
        matrixTransformacion[i][0][0] = cos(matrixDenavit[i][0] * toRad);//matrixDenavit[i][0]=0n
        //matrixTransformacion[i][0][0] = cos(90 * toRad);//matrixDenavit[i][0]=0n
        /*
        {Cos0n       zero           zero        zero  }
        {zero          1            zero        zero  }
        {zero        zero            1          zero  }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de Sen0n
        matrixTransformacion[i][1][0] = sin(matrixDenavit[i][0] * toRad);//matrixDenavit[i][0]=0n
        /*
        {Cos0n       zero           zero        zero  }
        {Sen0n         1            zero        zero  }
        {zero        zero            1          zero  }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de SenAn
        matrixTransformacion[i][2][1] = sin(matrixDenavit[i][3] * toRad);//matrixDenavit[i][3]=An
        /*
        {Cos0n       zero           zero        zero  }
        {Sen0n         1            zero        zero  }
        {zero        SenAn           1          zero  }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de CosAn
        matrixTransformacion[i][2][2] = cos(matrixDenavit[i][3] * toRad);//matrixDenavit[i][3]=An
        /*
        {Cos0n       zero           zero        zero  }
        {Sen0n         1            zero        zero  }
        {zero        SenAn          cosAn       zero  }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de dn
        matrixTransformacion[i][2][3] = matrixDenavit[i][1];//matrixDenavit[i][1]=dn
        /*
        {Cos0n       zero           zero        zero  }
        {Sen0n         1            zero        zero  }
        {zero        SenAn          cosAn        dn   }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de anCos0n
        matrixTransformacion[i][0][3] = cos(matrixDenavit[i][0] * toRad);//matrixDenavit[i][0]=0n
        matrixTransformacion[i][0][3] *= matrixDenavit[i][2];//matrixDenavit[i][2]=an
        /*
        {Cos0n       zero           zero      anCos0n }
        {Sen0n         1            zero        zero  }
        {zero        SenAn          cosAn        dn   }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de anSen0n
        matrixTransformacion[i][1][3] = sin(matrixDenavit[i][0] * toRad);//matrixDenavit[i][0]=0n
        matrixTransformacion[i][1][3] *= matrixDenavit[i][2];//matrixDenavit[i][2]=an
        /*
        {Cos0n       zero           zero      anCos0n }
        {Sen0n         1            zero      anSen0n }
        {zero        SenAn          cosAn        dn   }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de -Sen0nCosAn
        matrixTransformacion[i][0][1] = sin(matrixDenavit[i][0] * toRad);//matrixDenavit[i][0]=0n
        matrixTransformacion[i][0][1] *= cos(matrixDenavit[i][3] * toRad);//matrixDenavit[i][3]=An
        matrixTransformacion[i][0][1] *= -1;
        /*
        {Cos0n   -Sen0nCosAn        zero      anCos0n }
        {Sen0n         1            zero      anSen0n }
        {zero        SenAn          cosAn        dn   }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de Sen0nSenAn
        matrixTransformacion[i][0][2] = sin(matrixDenavit[i][0] * toRad);//matrixDenavit[i][0]=0n
        matrixTransformacion[i][0][2] *= sin(matrixDenavit[i][3] * toRad);//matrixDenavit[i][3]=An
        /*
        {Cos0n   -Sen0nCosAn     Sen0nSenAn   anCos0n }
        {Sen0n         1            zero      anSen0n }
        {zero        SenAn          cosAn        dn   }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de Cos0nCosAn
        matrixTransformacion[i][1][1] = cos(matrixDenavit[i][0] * toRad);//matrixDenavit[i][0]=0n
        matrixTransformacion[i][1][1] *= cos(matrixDenavit[i][3] * toRad);//matrixDenavit[i][3]=An
        /*
        {Cos0n   -Sen0nCosAn     Sen0nSenAn   anCos0n }
        {Sen0n    Cos0nCosAn        zero      anSen0n }
        {zero        SenAn          cosAn        dn   }
        {zero        zero           zero          1   }
        */

        //siguiente calculo de -Cos0nSenAn
        matrixTransformacion[i][1][2] = cos(matrixDenavit[i][0] * toRad);//matrixDenavit[i][0]=0n
        matrixTransformacion[i][1][2] *= sin(matrixDenavit[i][3] * toRad);//matrixDenavit[i][3]=An
        matrixTransformacion[i][1][2] *= -1;
        /*
        {Cos0n   -Sen0nCosAn     Sen0nSenAn   anCos0n }
        {Sen0n    Cos0nCosAn    -Cos0nSenAn   anSen0n }
        {zero        SenAn          cosAn        dn   }
        {zero        zero           zero          1   }
        */
    }
}
void printMatricesTransformacion() {
    for (int i = 0; i < numRows; i++)
    {
        printf("Matriz de transformacion T%d:\n", i + 1);
        for (int j = 0; j < numCols; j++)
        {
            for (int k = 0; k < numCols; k++)
            {
                printf("%f ", matrixTransformacion[i][j][k]);
            }printf("\n");
        }
        printf("\n\n");
    }
}
void generadorCoordenadas() {
    free(vectorCoordenadas);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i==j)
            {
                matrixResultante[i][j] = 1;
            }
            else {
                matrixResultante[i][j] = 0;
            }
        }
    }//volver la matriz resultante a valores default

    vectorCoordenadas = (float*)malloc(numRows*3 * sizeof(float));
    float matrixApoyo[4][4] = { {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };
    for (int i = 0; i < numRows; i++)
    {
        //printf("Multiplicacion %d\n", i);//debug
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                for (int l = 0; l < 4; l++)
                {
                    matrixApoyo[j][k] += matrixResultante[j][l] * matrixTransformacion[i][l][k];
                    
                }//printf("%f ", matrixApoyo[j][k]);//debug
            }//printf("\n");//debug
        }
        vectorCoordenadas[(i*3)] = matrixApoyo[0][3];
        vectorCoordenadas[(i*3)+1] = matrixApoyo[1][3];
        vectorCoordenadas[(i*3)+2] = matrixApoyo[2][3];

        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                matrixResultante[j][k] = matrixApoyo[j][k];
                matrixApoyo[j][k] = 0;
            }
        }
    }printf("matriz resultante\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%f ", matrixResultante[i][j]);
        }printf("\n");
    }
}
void imprimirCoordenadas() {
    printf("\nVector de coordenadas\n");
    for (int i = 0; i < numRows * 3; i++)
    {
        printf("%f ", vectorCoordenadas[i]);
    }
}
void simular() {
    free(matrixTransformacion);
    crearMatricesTransformacion();
    llenarMatricesTransformacion();
    printMatricesTransformacion();
    generadorCoordenadas();
    imprimirCoordenadas();
}

int main() {

    while (runing)
    {
        int eleccion = 1;
        printf("\nelija la opcion a realizar:\n1: Generar tabla Denavit-Hartenberg \n2: Editar las entradas\n3: Simular\n0: Salir del programa\n");
        scanf_s("%d", &eleccion);
        //erase pantalla
        switch (eleccion) {
        case 0:
            runing = false;
            break;
        case 1:
            llenadoMatrix4x4();
            printf("\nha elegido la opcion 1: Generar tabla Denavit-Hartenberg\n\n");
            numRows = 1;
            denavitMatrix();
            printMatrixDenavit();
            //printVectorEntradas();
            //printVariableVector();
            break;
        case 2:
            if (tablaGenerada==false)
            {
                printf("\nla opcion elegida no se puede realizar en estas condiciones, genere primero una tabla Denavit-Hartenberg\n");
            }else{
                printf("\nha elegido la opcion 2: Editar las entradas\n\n");
                cambioEntradas();
                printMatrixDenavit();
            }
            break;
        case 3:
            if (tablaGenerada == false)
            {
                printf("\nla opcion elegida no se puede realizar en estas condiciones, genere primero una tabla Denavit-Hartenberg\n");
            }
            else if (vectorZero==true&&tablaGenerada==true) {
                printf("\nLas entradas tienen valor zero, desea continuar? 0)No 1)Si\n\n");
                int userInput = 0;
                scanf_s("%d", &userInput);
                if (userInput==1)
                {
                    //printMatrixDenavit();
                    simular();
                    //printMatrixDenavit();
                }
                else {
                    break;
                }
            }
            else {
                printf("\nHas elegido la opcion 3: Simular\n\n");
                //printMatrixDenavit();
                simular();
                //printMatrixDenavit();
            }
            break;
        case 4:
            
            break;
        default:
            printf("\nNo se ha elegido una opcion valida\n\n");
            break;

        }
        
        
    }return 0;
}