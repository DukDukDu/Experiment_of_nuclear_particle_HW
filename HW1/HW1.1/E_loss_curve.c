TGraph* GetdEdxLise(string elossfile,int A)
{
  ifstream in(elossfile);//读入LISE软件生成的数据文件
  if(!in.is_open()){     //文件不存在时
    cout<<"Data File: "<<elossfile<< " does not exist!"<<endl;
    return 0;
  }
  auto *g=new TGraph;//定义新的二维散点图(TGraph) g
  string ss;
  double e, dedx;//因为我的文件只有两列（为了节省读入时间），因此设置两个变量就足够了
  
  getline(in, ss);//读入第一行文字避免循环时将字符读入到变量中
  int i=0;

  while(!in.eof())
    {
      in >> e >> dedx;
      getline(in,ss);//读取每一行剩余的数据，避免将空格或无用字符放入变量中

      if(in.eof()) break;//避免最后一个重复读取
      
      g->SetPoint(i++, e*A,dedx*1000);//添加数据点到图中，能量单位为MeV/u，需乘以核子数 A
    }
    
  in.close();
  return g;
}

void E_loss_curve(){

TCanvas *c=new TCanvas;
string elossfile1="c12_h2o.txt", elossfile2="p_h2o.txt";
TGraph *g_c12=GetdEdxLise(elossfile1,12);
TGraph *g_p=GetdEdxLise(elossfile2,1);

c->SetLogy();
c->SetLogx();

g_c12->SetTitle("dE/dx(MeV/mm) vs. MeV for 12C in water");//设定图的标题
g_c12->SetLineColor(kGreen);
//g_c12->GetXaxis()->SetLimits(-1, 0);
g_c12->SetMaximum(1e3);
g_c12->SetMinimum(1e-1);
g_c12->Draw();

g_p->SetLineColor(kRed);
g_p->Draw("same");

c->Draw();

cout << g_c12->Eval(0.) << endl;
}