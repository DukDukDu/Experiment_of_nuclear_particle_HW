TGraph* getdedx(string filename, int A){

    ifstream in(filename);

    auto *g = new TGraph;
    string s;
    double e, dedx;

    getline(in, s);
    int i = 0;

    while(!in.eof()){

        in >> e >> dedx;
        getline(in, s);
        if(in.eof()) break;

        g->SetPoint(i++, e*A, dedx);
    }

    in.close();
    return g;
}

double Z_1_8_dedx(string element, int A, double E){
    
    string a = "./data/";
    string b = "_si.txt";
    string c = a + element + b ;
    double dedx;  

    TGraph *z1 = getdedx(c, A);
    dedx = z1->Eval(E, 0, "S");
    

    
    return dedx;

}