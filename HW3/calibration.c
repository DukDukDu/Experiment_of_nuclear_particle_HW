double fun(double *x, double *par){

    return par[0]*TMath::Exp(-0.5*((x[0]-par[1])/par[2]) * ((x[0]-par[1])/par[2])) + par[3]+x[0]*par[4];
}

void Fit(TH1F *hist, double height, double middle, double r_fit, double l_fit, double r_show, double l_show){
    TF1 *ffit = new TF1("ffit", fun, 0, 2500, 5);
    ffit->SetParameter(0,height);//par[0]-高斯函数高度
    ffit->SetParameter(1, middle);//par[1]-高斯函数峰中心
    ffit->SetParameter(2,1.0);//par[2]-高斯函数sigma
    ffit->SetParameter(3,1.0e3);//par[3]-线性函数零次项
    ffit->SetParameter(4,0.01);//par[4]-线性函数一次项
    
    hist->GetXaxis()->SetRangeUser(0, 2500);
    hist->Fit("ffit", " ", " ", r_fit, l_fit);
    hist->Draw();
    hist->GetXaxis()->SetRangeUser(r_show, l_show);
}

void calibration(){

    TFile *filer = new TFile("gamma.root","READ");// 以只读模式打开ROOT文件
    if(!filer->IsOpen()){

        std::cout<<"Can't open root file"<<std::endl;
  }

    TH1F *h_gammaraw = (TH1F*)filer->Get("h0");//通过指针读取原始gamma谱

    Fit(h_gammaraw, 1e5, 1217, 1211, 1223, 1208, 1226);

}
