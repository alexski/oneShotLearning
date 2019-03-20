#include<stdio.h>
#include<stdlib.h>

double** inverse(double**, int, int);

int main(int argc, char** argv) {
    FILE* train;
    FILE* data;
    double num;
    int i, j, rowD, colD, rowT, colT;
    double** datmat;
    double** X;
    double** XT;
    double** XTX;
    double** XTXxt;
    double* Y;
    double* W;

    if(argc != 3){//3 arguments or Error
        return 0;
    }
    if(!(train = fopen(argv[1], "r"))){ //point to train
        printf("0\n");
        return 0;
    }else{
        train = fopen(argv[1], "r");
    }

    if(fscanf(train, "%d", &colD) != EOF);
    if(fscanf(train, "%d", &rowT) != EOF);
    colT = colD;
    colT++;

    if(colD == 0 || rowT == 0){
        fclose(train);
        return 0;
    }

    //BUILD MATRICES
    X = (double**) malloc(rowT * sizeof(double*));
    for(i = 0; i < rowT; i++){
        X[i] = (double*) malloc(colT * sizeof(double));
    }
    XT = (double**) malloc(colT * sizeof(double*));
    for(i = 0; i < colT; i++){
        XT[i] = (double*) malloc(rowT * sizeof(double));
    }
    XTX = (double**) malloc(colT * sizeof(double*));
    for(i = 0; i < colT; i++){
        XTX[i] = (double*) malloc(colT * sizeof(double));
    }
    XTXxt = (double**) malloc(colT * sizeof(double*));
    for(i = 0; i < colT; i++){
        XTXxt[i] = (double*) malloc(rowT * sizeof(double));
    }
    Y = (double*) malloc(rowT * sizeof(double));
    W = (double*) malloc(colT * sizeof(double));

    //LEADING 1 IN X MATRIX
    for(i = 0; i < rowT; i++){
        X[i][0] = 1;
    }

    //STORE TRAINING FILE INTO X MATRIX, XT MATRIX, AND Y MATRIX
    for(i = 0; i < rowT; i++){
        for(j = 1; j <= colT; j++){
            if(fscanf(train, "%lf,", &num) != EOF){
                if(j == colT){
                    Y[i] = num;
                }else{
                    X[i][j] = num;
                }
            }else{
                return 0;
            }
        }
    }
    for(i = 0; i < rowT; i++){
        for(j = 0; j < colT; j++){
            XT[j][i] = X[i][j];
        }
    }

    fclose(train);

    //MULTIPLIES XT * X = XTX MATRIX
    for(i = 0; i < colT; i++){
        for(j = 0; j < colT; j++){
            int curr = 0;
            double total = 0.0;
            while(curr < rowT){
                total = total + (XT[i][curr] * X[curr][j]);
                curr = curr + 1;
            }
            XTX[i][j] = total;
        }
    }

    XTX = inverse(XTX, colT, colT);

    //MULTIPLIES XTX * XT = XTXxt MATRIX
    for(i = 0; i < colT; i++){
        for(j = 0; j < rowT; j++){
            int curr = 0;
            double total = 0;
            while(curr < colT){
                total = total + (XTX[i][curr] * XT[curr][j]);
                curr = curr + 1;
            }
            XTXxt[i][j] = total;
        }
    }

    //MULTIPLIES XTXxt * Y = W MATRIX
    for(i = 0; i < colT; i++){
        int curr = 0;
        double total = 0;
        while(curr < rowT){
            total = total + (XTXxt[i][curr] * Y[curr]);
            curr = curr + 1;
        }
        W[i] = total;
    }

    if(!(data = fopen(argv[2], "r"))){ //point to data
        printf("0\n");
        return 0;
    }else{
        data = fopen(argv[2], "r");
    }

    if(fscanf(data, "%d", &rowD) != EOF);

    if(rowD == 0){
        fclose(data);
        for(i = 0; i < rowT; i++){ //Free X
            free(X[i]);
        }
        for(i = 0; i < colT; i++){ //Free XT, XTX, and XTXxt
            free(XT[i]);
            free(XTX[i]);
            free(XTXxt[i]);
        }
        free(X);
        free(XT);
        free(XTX);
        free(Y);
        free(W);
        return 0;
    }

    //BUILD DATMAT WITH DATA FILE INPUTS
    datmat = (double**) malloc(rowD * sizeof(double*));
    for(i = 0; i < rowD; i++){
        datmat[i] = (double*) malloc(colT * sizeof(double));
    }
	//INPUT LEADING 1 COLUMN FOR DATMAT
    for(i = 0; i < rowD; i++){
        datmat[i][0] = 1;
    }

    //STORE DATA FILE INPUT INTO DATMAT
    for(i = 0; i < rowD; i++){
        for(j = 1; j < colT; j++){
            if(fscanf(data, "%lf,", &num) != EOF){
                datmat[i][j] = num;
            }else{
                return 0;
            }
        }
    }
    fclose(data);

    double* result = (double*) malloc(rowD * sizeof(double));

    //MULTIPLIES DATMAT AND W TO GET RESULT MATRIX
    for(i = 0; i < rowD; i++){
        int curr = 0;
        double total = 0;
        while(curr < colT){
            total = total + (datmat[i][curr] * W[curr]);
            curr = curr + 1;
        }
        result[i] = total;
    }

    for(i = 0; i < colT; i++){
        int curr = 0;
        double total = 0;
        while(curr < rowT){
            total = total + (XTXxt[i][curr] * Y[curr]);
            curr = curr + 1;
        }
        W[i] = total;
    }

	//PRINTS PRICES OF DATA HOUSES
    for(i = 0; i < rowD; i++){
        printf("%0.0lf\n", result[i]);
    }

    for(i = 0; i < rowT; i++){ //Free X
        free(X[i]);
    }
    for(i = 0; i < colT; i++){ //Free XT, XTX, and XTXxt
        free(XT[i]);
        free(XTX[i]);
        free(XTXxt[i]);
    }
    for(i = 0; i < rowD; i++){ //Free datmat
        free(datmat[i]);
    }
    free(X);
    free(XT);
    free(XTX);
    free(Y);
    free(W);
    free(datmat);
    free(result);
    return 0;
}

double** inverse(double** original, int row, int col){
    int i, j, k;
    double** aug = (double**) malloc(row * sizeof(double*));
    for(i = 0; i < row; i++){
        aug[i] = (double*) malloc((2*col) * sizeof(double));
    }
    //copys original to aug
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            aug[i][j] = original[i][j];
        }
    }

    //adds the identity matrix into the aug matrix
    for(i = 0; i < row; i++){
        for(j = col; j < (2*col); j++){
            if((col + i) == j){
                aug[i][j] = 1.0;
            }else{
                aug[i][j] = 0.0;
            }
        }
    }

    if(aug[0][0] != 1.0){
        double val = aug[0][0];
        for(j = 0; j < (2*col); j++){
            aug[0][j] = (aug[0][j] / val);
        }
    }

    //BOTTOM TRIANGLE OF ZEROES
    for(i = 0; i < row; i++){
        for(j = 0; j <= i; j++){
            if(aug[i][j] == 0.0){
                continue;
            }
            if(i == j && aug[i][j] == 1.0){
                continue;
            }
            if(aug[i][j] == 1.0){
                for(k = 0; k < (2*col); k++){
                    aug[i][k] = aug[i][k] - aug[j][k];
                }
                continue;
            }

            double val = aug[i][j];
            for(k = 0; k < (2*col); k++){
                aug[i][k] = (aug[i][k] / val);
            }
            if(i == j){
                continue;
            }else{
                for(k = 0; k < (2*col); k++){
                    aug[i][k] = aug[i][k] - aug[j][k];
                }
            }
        }
    }

    //TOP ZERO TRIANGLE
    for(i = row-1; i >= 0; i--){
        for(j = col-1; j >= 0; j--){
            if(aug[i][j] == 0){
                continue;
            }
            if(aug[i][j] == 1){
                continue;
            }else{
                double val = aug[i][j];
                for(k = 0; k < (2*col); k++){
                    aug[i][k] = aug[i][k] - val * aug[j][k];
                }
            }
        }
    }

    //REPLACES THE ORIGINAL MATRIX WITH ITS INVERSE FROM THE AUG MATRIX
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            original[i][j] = aug[i][j+col];
        }
    }

    free(aug);
    return original;
}
