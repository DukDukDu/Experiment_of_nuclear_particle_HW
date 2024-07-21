TGraph* GetdEdxLise(string elossfile,int A)
{
  ifstream in(elossfile);
  if(!in.is_open()){     
    cout<<"Data File: "<<elossfile<< " does not exist!"<<endl;
    return 0;
  }
  auto *g=new TGraph;
  string ss;
  double e, dedx;
  
  getline(in, ss);
  int i=0;

  while(!in.eof())
    {
      in >> e >> dedx;
      getline(in,ss);

      if(in.eof()) break;
      
      g->SetPoint(i++, e*A, dedx);
    }
    
  in.close();
  return g;
}



double take_data_p(double x){

    TGraph *p_h2o = GetdEdxLise("p_h2o.txt", 1);
    
    double de_dx_p = p_h2o->Eval(x, 0, "S");

    return de_dx_p;
}

double take_data_c(double x){

    TGraph *c12_h2o = GetdEdxLise("c12_h2o.txt", 12);

    double de_dx_c = c12_h2o->Eval(x, 0, "S");

    return de_dx_c;
}