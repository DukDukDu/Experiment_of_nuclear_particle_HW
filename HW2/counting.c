TH1F* distri(string filename, TH1F *hist){

    ifstream in(filename);
    
    string s;
    float count_num, count_res, space;

    getline(in, s);

    int i = 0;

    while(!in.eof()){

        in >> count_num >> count_res;
        getline(in, s);
        if(in.eof()) break;

        hist->Fill(count_res);

    }

    in.close();
    return hist;
}

TH1F* interval_per_five(string filename, TH1F *hist){

    ifstream in(filename);
    
    string s;
    float count_num, count_res, space;
    double interval_5;

    getline(in, s);

    int i = 0;

    while(!in.eof()){
        i = i + 1;
        in >> count_num >> count_res;
        interval_5 = interval_5 + count_res;
        getline(in, s);
        if(in.eof()) break;

        if (i%5 == 0){
            hist->Fill(interval_5);
            interval_5 = 0;
        }
    }

    in.close();
    return hist;
}

void counting(){

    string f1 = "high_counts.txt";
    string f2 = "low_counts.txt";
    string f3 = "counts_per_second.txt";
    string f4 = "time_interval.txt";

    TH1F *high_hist = new TH1F("high", " ", 25, 400, 900);
    TH1F *low_hist = new TH1F("low", " ", 15, 0, 15);

    TH1F *per_sec = new TH1F("per_sec", " ", 40, 600, 1000);
    TH1F *time_interval = new TH1F("time_interval", " ", 100, 0, 5000);
    TH1F *interval5 = new TH1F("interval5", " ", 200, 0, 20000);

    TF1 *poisson = new TF1("poisson", "[0]*TMath::PoissonI(x, [1])", 0, 15);
    poisson->SetParameters(1, 3);
    poisson->SetParLimits(1, 1, 5);

    TCanvas *c1 = new TCanvas("c1", " ", 800, 600);
    c1->Divide(2,1);
    c1->cd(1);
    distri(f1, high_hist)->Draw();
    distri(f1, high_hist)->Fit("gaus");

    c1->cd(2);
    //distri(f2, low_hist)->Scale(1/distri(f2, low_hist)->Integral());;
    distri(f2, low_hist)->Draw();
    distri(f2, low_hist)->Fit("poisson","R");
    poisson->Draw("same");

    TCanvas *c2 = new TCanvas("c2", " ", 800, 600);
    c2->Divide(3, 1);
    c2->cd(1);
    distri(f3, per_sec)->GetXaxis()->SetTitle("counts per sec");
    distri(f3, per_sec)->Draw();

    c2->cd(2);
    distri(f4, time_interval)->GetXaxis()->SetTitle("time interval");
    distri(f4, time_interval)->Draw();
    distri(f4, time_interval)->Fit("expo");
    
    c2->cd(3);
    interval_per_five(f4, interval5)->GetXaxis()->SetTitle("time interval every 5 events");
    interval_per_five(f4, interval5)->Draw();
}