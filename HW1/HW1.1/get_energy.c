#include "take_data.h"

void get_energy(){
    
    int n0 = 1270;
    double E1 = 0., E2 = 0.;

    for (int i = 0; i < 1e5; i++){

        double E_loss_p;
        E_loss_p = take_data_p(E1);
        
        E1 = E1 + 10*take_data_p(E1);

        if (take_data_p(E1) < E_loss_p) break;

    }

    for(int i1 = 0; i1 < 12700; i1++){

        E1 = E1 + 10*take_data_p(E1);
    }

    for(int j = 0; j < 1e5; j++){

        double E_loss_c;
        E_loss_c = take_data_c(E2);

        E2 = E2 + 1*take_data_c(E2);

        if (take_data_c(E2) < E_loss_c) break;
    }

    for (int j1 = 0; j1 < 127000; j1++){

        E2 = E2 +1*take_data_c(E2);
    }

    cout << E1 << " " << E2 << endl;
}
