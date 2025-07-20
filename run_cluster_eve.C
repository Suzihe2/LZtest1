void run_cluster_eve(TString inFile_DAQ_TPC="/public/CEEDAQ/BeamTest_Nov2023/Beam_Data/data_20231205_20/DAS2_2/data_2_20231205T112631_0.dat", Int_t nEvent = 30)
{

	Int_t lastSlashIndex = inFile_DAQ_TPC.Last('/');//获取inFile_DAQ_TPC最后一个/的位置
 	TString DAQfilename = inFile_DAQ_TPC(lastSlashIndex + 1, inFile_DAQ_TPC.Length() - lastSlashIndex - 1);//获取文件名，把前面目录都删掉
	//TString parFile = "params.root";
	TString inFile = DAQfilename.ReplaceAll(".dat","_tpcdigi.root");//把文件名（DAQfilename）里的.dat替换成_tpcdigi.root；
 	TString outFile = DAQfilename.ReplaceAll("_tpcdigi.root","_tpchit.root");//把文件名（DAQfilename）里的_tpcdigi.root替换成_tpchit.root；
	TStopwatch timer;
	timer.Start();

	//------------cluster reconstruction run---------------------------------------------

	FairRunAna *fRun= new FairRunAna();
	fRun->SetInputFile(inFile);   //只是把字符串inFile（即文件名）存进fRun;
	fRun->SetOutputFile(outFile); //告诉fRun,所有task处理完写到这个文件名里；


	//-----------------------------------------------------------------------------------
	//FairRuntimeDb*  rtdb = fRun->GetRuntimeDb();
	//FairParRootFileIo* parInput1 = new FairParRootFileIo();
	//parInput1->open(parFile.Data());

	//  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
	//  parIo1->open(digiFile.Data(),"in");
	//rtdb->setFirstInput(parInput1);
	//  rtdb->setSecondInput(parIo1);
	//rtdb->Print();


	//============cluster Sequence=======================================================

	CeeTpcClusterFinderTask*clusterFinder = new CeeTpcClusterFinderTask();
	clusterFinder->SetPersistence();//持久化开关，指要把任务处理完的结果保存到输出文件；不调用这个函数，任务会执行，但不会自动保存到输出文件；
	clusterFinder->SetDebug(kTRUE)//是否打印详细的调试日志;
	clusterFinder->SetMakeQA();//clusterFinder在执行任务时会统计并填充各种QA直方图；
	fRun->AddTask(clusterFinder);//把clusterFinder添加到fRun里；

	//============intialise and run======================================================

	fRun->Init();//将前面配置好的task串起来，
	fRun->Run(0,nEvent);//开始事件处理循环；

	//-----------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------
	timer.Stop();
	Double_t rtime = timer.RealTime();
	Double_t ctime = timer.CpuTime();
	cout << endl << endl;
	cout << "Cluster task finished successfully"<<endl;
	cout << "Output file is "    << outFile << endl;
	cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
	cout << endl;


	// exit(0);
} 

