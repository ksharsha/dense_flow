#include "dense_flow.h"
#include "utils.h"

INITIALIZE_EASYLOGGINGPP

using namespace cv::gpu;

int main(int argc, char** argv){
	// IO operation
	const char* keys =
		{
			"{ f  | vidFile      | ex2.avi | filename of video }"
			"{ x  | xFlowFile    | flow_x | filename of flow x component }"
			"{ y  | yFlowFile    | flow_y | filename of flow x component }"
			"{ i  | imgFile      | flow_i | filename of flow image}"
			"{ b  | bound | 15 | specify the maximum of optical flow}"
			"{ t  | type | 0 | specify the optical flow algorithm }"
			"{ d  | device_id    | 0  | set gpu id}"
			"{ s  | step  | 1 | specify the step for frame sampling}"
			"{ o  | out | zip | output style}"
			"{ w  | newWidth | 0 | output style}"
			"{ h  | newHeight | 0 | output style}"
		};

	CommandLineParser cmd(argc, argv, keys);
	string vidFile = cmd.get<string>("vidFile");
	string xFlowFile = cmd.get<string>("xFlowFile");
	string yFlowFile = cmd.get<string>("yFlowFile");
	string imgFile = cmd.get<string>("imgFile");
	string output_style = cmd.get<string>("out");
	int bound = cmd.get<int>("bound");
    int type  = cmd.get<int>("type");
    int device_id = cmd.get<int>("device_id");
    int step = cmd.get<int>("step");
    int new_height = cmd.get<int>("newHeight");
    int new_width = cmd.get<int>("newWidth");

	vector<vector<uchar> > out_vec_x, out_vec_y, out_vec_img;

	calclongtimeDenseFlowGPU(vidFile, bound, type, step, device_id,
					 out_vec_x, out_vec_y, out_vec_img, new_width, new_height);

	if (output_style == "dir") {
		writeImages(out_vec_x, xFlowFile);
		writeImages(out_vec_y, yFlowFile);
		writeImages(out_vec_img, imgFile);
	}else{
//		LOG(INFO)<<"Writing results to Zip archives";
		writeZipFile(out_vec_x, "x_%05d.jpg", xFlowFile+".zip");
		writeZipFile(out_vec_y, "y_%05d.jpg", yFlowFile+".zip");
		writeZipFile(out_vec_img, "img_%05d.jpg", imgFile+".zip");
	}

	return 0;
}
