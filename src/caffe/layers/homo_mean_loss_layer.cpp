#include "caffe/layers/homo_mean_loss_layer.hpp"
#include "caffe/util/math_functions.hpp"
#include "caffe/messenger.hpp"

namespace caffe {

template <typename Dtype>
class SolverIterChangedHandlerForHomoMeanLossLayer : public Listener {
public:
  SolverIterChangedHandlerForHomoMeanLossLayer(HomoMeanLossLayer<Dtype>* homo_mean_loss_layer)
    : homo_mean_loss_layer_(homo_mean_loss_layer) {
  }
  void handle(const void* message) {
    homo_mean_loss_layer_->SolverIterChangedHandle(message);
  }
private:
  HomoMeanLossLayer<Dtype>* homo_mean_loss_layer_;
};

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  HomoMeanLossParameter homo_mean_loss_param = this->layer_param_.homo_mean_loss_param();
  predict_measure_  = homo_mean_loss_param.predict_measure();
  cluster_measure_  = homo_mean_loss_param.cluster_measure();
  scatter_measure_  = homo_mean_loss_param.scatter_measure();
  clusupd_measure_  = homo_mean_loss_param.clusupd_measure();
  scatupd_measure_  = homo_mean_loss_param.scatupd_measure();
  odotter_measure_  = homo_mean_loss_param.odotter_measure();
  ovalize_measure_  = homo_mean_loss_param.ovalize_measure();
  cluster_regular_  = homo_mean_loss_param.cluster_regular();
  scatter_regular_  = homo_mean_loss_param.scatter_regular();
  odotter_regular_  = homo_mean_loss_param.odotter_regular();
  ovalize_regular_  = homo_mean_loss_param.ovalize_regular();
  cluster_clipmode_ = homo_mean_loss_param.cluster_clipmode();
  scatter_clipmode_ = homo_mean_loss_param.scatter_clipmode();
  topdiff_clipmode_ = homo_mean_loss_param.topdiff_clipmode();
  ovalize_clipmode_ = homo_mean_loss_param.ovalize_clipmode();
  cluster_clipactv_ = homo_mean_loss_param.cluster_clipactv();
  scatter_clipactv_ = homo_mean_loss_param.scatter_clipactv();
  topdiff_clipactv_ = homo_mean_loss_param.topdiff_clipactv();
  ovalize_clipactv_ = homo_mean_loss_param.ovalize_clipactv();
  cluster_clipnorm_ = homo_mean_loss_param.cluster_clipnorm();
  scatter_clipnorm_ = homo_mean_loss_param.scatter_clipnorm();
  topdiff_clipnorm_ = homo_mean_loss_param.topdiff_clipnorm();
  ovalize_clipnorm_ = homo_mean_loss_param.ovalize_clipnorm();
  cluster_clipprop_ = homo_mean_loss_param.cluster_clipprop();
  cluster_lowrprop_ = homo_mean_loss_param.cluster_lowrprop();
  cluster_upprprop_ = homo_mean_loss_param.cluster_upprprop();
  cluster_feedrate_ = homo_mean_loss_param.cluster_feedrate();
  cluster_feedsize_ = homo_mean_loss_param.cluster_feedsize();
  cluster_tradeoff_ = homo_mean_loss_param.cluster_tradeoff();
  scatter_clipprop_ = homo_mean_loss_param.scatter_clipprop();
  scatter_lowrprop_ = homo_mean_loss_param.scatter_lowrprop();
  scatter_upprprop_ = homo_mean_loss_param.scatter_upprprop();
  scatter_feedrate_ = homo_mean_loss_param.scatter_feedrate();
  scatter_feedsize_ = homo_mean_loss_param.scatter_feedsize();
  scatter_tradeoff_ = homo_mean_loss_param.scatter_tradeoff();
  topdiff_clipprop_ = homo_mean_loss_param.topdiff_clipprop();
  topdiff_lowrprop_ = homo_mean_loss_param.topdiff_lowrprop();
  topdiff_upprprop_ = homo_mean_loss_param.topdiff_upprprop();
  topdiff_feedrate_ = homo_mean_loss_param.topdiff_feedrate();
  topdiff_feedsize_ = homo_mean_loss_param.topdiff_feedsize();
  topdiff_tradeoff_ = homo_mean_loss_param.topdiff_tradeoff();
  ovalize_clipprop_ = homo_mean_loss_param.ovalize_clipprop();
  ovalize_lowrprop_ = homo_mean_loss_param.ovalize_lowrprop();
  ovalize_upprprop_ = homo_mean_loss_param.ovalize_upprprop();
  ovalize_feedrate_ = homo_mean_loss_param.ovalize_feedrate();
  ovalize_feedsize_ = homo_mean_loss_param.ovalize_feedsize();
  ovalize_tradeoff_ = homo_mean_loss_param.ovalize_tradeoff();
  ovals2s_01stprop_ = homo_mean_loss_param.ovals2s_01stprop();
  ovals2s_02ndprop_ = homo_mean_loss_param.ovals2s_02ndprop();
  ovalt2t_01stprop_ = homo_mean_loss_param.ovalt2t_01stprop();
  ovalt2t_02ndprop_ = homo_mean_loss_param.ovalt2t_02ndprop();
  ovals2t_01stprop_ = homo_mean_loss_param.ovals2t_01stprop();
  ovals2t_02ndprop_ = homo_mean_loss_param.ovals2t_02ndprop();
  cluster_interval_ = homo_mean_loss_param.cluster_interval();
  scatter_interval_ = homo_mean_loss_param.scatter_interval();
  topdiff_interval_ = homo_mean_loss_param.topdiff_interval();
  ovalize_interval_ = homo_mean_loss_param.ovalize_interval();
  cluster_postpone_ = homo_mean_loss_param.cluster_postpone();
  scatter_postpone_ = homo_mean_loss_param.scatter_postpone();
  topdiff_postpone_ = homo_mean_loss_param.topdiff_postpone();
  ovalize_postpone_ = homo_mean_loss_param.ovalize_postpone();
  cluster_duration_ = homo_mean_loss_param.cluster_duration();
  scatter_duration_ = homo_mean_loss_param.scatter_duration();
  topdiff_duration_ = homo_mean_loss_param.topdiff_duration();
  ovalize_duration_ = homo_mean_loss_param.ovalize_duration();
  cluster_normalization_ = homo_mean_loss_param.cluster_normalization();
  scatter_normalization_ = homo_mean_loss_param.scatter_normalization();
  ovalize_normalization_ = homo_mean_loss_param.ovalize_normalization();
  average_initmode_ = homo_mean_loss_param.average_initmode();
  match_numb_ = homo_mean_loss_param.match_numb();
  label_nmax_ = homo_mean_loss_param.label_nmax();
  label_axis_ = bottom[0]->CanonicalAxisIndex(homo_mean_loss_param.label_axis());
  outer_numb_ = bottom[0]->count(0, label_axis_);
  inner_numb_ = bottom[0]->count() / outer_numb_;
  label_numb_ = bottom[1]->count() / outer_numb_;
  match_numb_ = match_numb_ ? match_numb_ : outer_numb_;
  target_label_ = homo_mean_loss_param.target_label();
  ignore_label_ = homo_mean_loss_param.ignore_label();
  cluster_clipprop_ = std::max(Dtype(1e-7), cluster_clipprop_ / ClusterNormalizer());
  scatter_clipprop_ = std::max(Dtype(1e-7), scatter_clipprop_ / ScatterNormalizer());
  ovalize_clipprop_ = std::max(Dtype(1e-7), ovalize_clipprop_ / OvalizeNormalizer());
  int blobs_count = 0;
  outputs_activate_.push_back((homo_mean_loss_param.replica_activate() && blobs_count < top.size()) ? blobs_count++ : -1);
  outputs_activate_.push_back((homo_mean_loss_param.predict_activate() && blobs_count < top.size()) ? blobs_count++ : -1);
  outputs_activate_.push_back((homo_mean_loss_param.cluster_activate() && blobs_count < top.size()) ? blobs_count++ : -1);
  outputs_activate_.push_back((homo_mean_loss_param.scatter_activate() && blobs_count < top.size()) ? blobs_count++ : -1);
  outputs_activate_.push_back((homo_mean_loss_param.clusupd_activate() && blobs_count < top.size()) ? blobs_count++ : -1);
  outputs_activate_.push_back((homo_mean_loss_param.scatupd_activate() && blobs_count < top.size()) ? blobs_count++ : -1);
  outputs_activate_.push_back((homo_mean_loss_param.ovalize_activate() && blobs_count < top.size()) ? blobs_count++ : -1);
  CHECK(predict_measure_ == "rawsubsqr-overall-average" || predict_measure_ == "rawsubsqr-nonself-average" ||
        predict_measure_ == "logsubsqr-overall-average" || predict_measure_ == "logsubsqr-nonself-average" ||
        predict_measure_ == "expsubsqr-overall-average" || predict_measure_ == "expsubsqr-nonself-average" ||
        predict_measure_ == "rawsubabs-overall-average" || predict_measure_ == "rawsubabs-nonself-average" ||
        predict_measure_ == "logsubabs-overall-average" || predict_measure_ == "logsubabs-nonself-average" ||
        predict_measure_ == "expsubabs-overall-average" || predict_measure_ == "expsubabs-nonself-average")
      << "illegal predict measure: " << predict_measure_ << "!";
  CHECK(cluster_measure_ == "rawsubsqr-overall-average" || cluster_measure_ == "rawsubsqr-nonself-average" ||
        cluster_measure_ == "logsubsqr-overall-average" || cluster_measure_ == "logsubsqr-nonself-average" ||
        cluster_measure_ == "expsubsqr-overall-average" || cluster_measure_ == "expsubsqr-nonself-average" ||
        cluster_measure_ == "rawsubabs-overall-average" || cluster_measure_ == "rawsubabs-nonself-average" ||
        cluster_measure_ == "logsubabs-overall-average" || cluster_measure_ == "logsubabs-nonself-average" ||
        cluster_measure_ == "expsubabs-overall-average" || cluster_measure_ == "expsubabs-nonself-average")
      << "illegal cluster measure: " << cluster_measure_ << "!";
  CHECK(scatter_measure_ == "rawsubsqr-samples-average" || scatter_measure_ == "rawsubsqr-average-average" ||
        scatter_measure_ == "logsubsqr-samples-average" || scatter_measure_ == "logsubsqr-average-average" ||
        scatter_measure_ == "expsubsqr-samples-average" || scatter_measure_ == "expsubsqr-average-average" ||
        scatter_measure_ == "rawsubabs-samples-average" || scatter_measure_ == "rawsubabs-average-average" ||
        scatter_measure_ == "logsubabs-samples-average" || scatter_measure_ == "logsubabs-average-average" ||
        scatter_measure_ == "expsubabs-samples-average" || scatter_measure_ == "expsubabs-average-average")
      << "illegal scatter measure: " << scatter_measure_ << "!";
  CHECK(clusupd_measure_ == "rawsubsqr-overall-average" || clusupd_measure_ == "rawsubsqr-nonself-average" ||
        clusupd_measure_ == "logsubsqr-overall-average" || clusupd_measure_ == "logsubsqr-nonself-average" ||
        clusupd_measure_ == "expsubsqr-overall-average" || clusupd_measure_ == "expsubsqr-nonself-average" ||
        clusupd_measure_ == "rawsubabs-overall-average" || clusupd_measure_ == "rawsubabs-nonself-average" ||
        clusupd_measure_ == "logsubabs-overall-average" || clusupd_measure_ == "logsubabs-nonself-average" ||
        clusupd_measure_ == "expsubabs-overall-average" || clusupd_measure_ == "expsubabs-nonself-average")
      << "illegal clusupd measure: " << clusupd_measure_ << "!";
  CHECK(scatupd_measure_ == "rawsubsqr-samples-average" || scatupd_measure_ == "rawsubsqr-average-average" ||
        scatupd_measure_ == "logsubsqr-samples-average" || scatupd_measure_ == "logsubsqr-average-average" ||
        scatupd_measure_ == "expsubsqr-samples-average" || scatupd_measure_ == "expsubsqr-average-average" ||
        scatupd_measure_ == "rawsubabs-samples-average" || scatupd_measure_ == "rawsubabs-average-average" ||
        scatupd_measure_ == "logsubabs-samples-average" || scatupd_measure_ == "logsubabs-average-average" ||
        scatupd_measure_ == "expsubabs-samples-average" || scatupd_measure_ == "expsubabs-average-average")
      << "illegal scatupd measure: " << scatupd_measure_ << "!";
  CHECK(odotter_measure_ == "rawsubsqr-overall-average" || odotter_measure_ == "rawsubsqr-nonself-average" ||
        odotter_measure_ == "logsubsqr-overall-average" || odotter_measure_ == "logsubsqr-nonself-average" ||
        odotter_measure_ == "expsubsqr-overall-average" || odotter_measure_ == "expsubsqr-nonself-average" ||
        odotter_measure_ == "rawsubabs-overall-average" || odotter_measure_ == "rawsubabs-nonself-average" ||
        odotter_measure_ == "logsubabs-overall-average" || odotter_measure_ == "logsubabs-nonself-average" ||
        odotter_measure_ == "expsubabs-overall-average" || odotter_measure_ == "expsubabs-nonself-average")
      << "illegal odotter measure: " << odotter_measure_ << "!";
  CHECK(ovalize_measure_ == "rawsubsqr-origins-origins" || ovalize_measure_ == "rawsubsqr-sqroots-sqroots" ||
        ovalize_measure_ == "logsubsqr-origins-origins" || ovalize_measure_ == "logsubsqr-sqroots-sqroots" ||
        ovalize_measure_ == "expsubsqr-origins-origins" || ovalize_measure_ == "expsubsqr-sqroots-sqroots" ||
        ovalize_measure_ == "rawsubabs-origins-origins" || ovalize_measure_ == "rawsubabs-sqroots-sqroots" ||
        ovalize_measure_ == "logsubabs-origins-origins" || ovalize_measure_ == "logsubabs-sqroots-sqroots" ||
        ovalize_measure_ == "expsubabs-origins-origins" || ovalize_measure_ == "expsubabs-sqroots-sqroots")
      << "illegal ovalize measure: " << ovalize_measure_ << "!";
  CHECK(cluster_regular_ == "rawsubsqr-overall-cluster" || cluster_regular_ == "rawsubsqr-nonself-cluster" ||
        cluster_regular_ == "rawsubsqr-overall-average" || cluster_regular_ == "rawsubsqr-nonself-average" ||
        cluster_regular_ == "logsubsqr-overall-average" || cluster_regular_ == "logsubsqr-nonself-average" ||
        cluster_regular_ == "expsubsqr-overall-average" || cluster_regular_ == "expsubsqr-nonself-average" ||
        cluster_regular_ == "rawsubabs-overall-average" || cluster_regular_ == "rawsubabs-nonself-average" ||
        cluster_regular_ == "logsubabs-overall-average" || cluster_regular_ == "logsubabs-nonself-average" ||
        cluster_regular_ == "expsubabs-overall-average" || cluster_regular_ == "expsubabs-nonself-average")
      << "illegal cluster regular: " << cluster_regular_ << "!";
  CHECK(scatter_regular_ == "rawsubsqr-samples-cluster" || scatter_regular_ == "rawsubsqr-cluster-cluster" ||
        scatter_regular_ == "rawsubsqr-samples-average" || scatter_regular_ == "rawsubsqr-average-average" ||
        scatter_regular_ == "logsubsqr-samples-average" || scatter_regular_ == "logsubsqr-average-average" ||
        scatter_regular_ == "expsubsqr-samples-average" || scatter_regular_ == "expsubsqr-average-average" ||
        scatter_regular_ == "rawsubabs-samples-average" || scatter_regular_ == "rawsubabs-average-average" ||
        scatter_regular_ == "logsubabs-samples-average" || scatter_regular_ == "logsubabs-average-average" ||
        scatter_regular_ == "expsubabs-samples-average" || scatter_regular_ == "expsubabs-average-average")
      << "illegal scatter regular: " << scatter_regular_ << "!";
  CHECK(odotter_regular_ == "rawsubsqr-overall-average" || odotter_regular_ == "rawsubsqr-nonself-average" ||
        odotter_regular_ == "logsubsqr-overall-average" || odotter_regular_ == "logsubsqr-nonself-average" ||
        odotter_regular_ == "expsubsqr-overall-average" || odotter_regular_ == "expsubsqr-nonself-average" ||
        odotter_regular_ == "rawsubabs-overall-average" || odotter_regular_ == "rawsubabs-nonself-average" ||
        odotter_regular_ == "logsubabs-overall-average" || odotter_regular_ == "logsubabs-nonself-average" ||
        odotter_regular_ == "expsubabs-overall-average" || odotter_regular_ == "expsubabs-nonself-average")
      << "illegal odotter regular: " << odotter_regular_ << "!";
  CHECK(ovalize_regular_ == "rawsubsqr-origins-origins" || ovalize_regular_ == "rawsubsqr-sqroots-sqroots" ||
        ovalize_regular_ == "logsubsqr-origins-origins" || ovalize_regular_ == "logsubsqr-sqroots-sqroots" ||
        ovalize_regular_ == "expsubsqr-origins-origins" || ovalize_regular_ == "expsubsqr-sqroots-sqroots" ||
        ovalize_regular_ == "rawsubabs-origins-origins" || ovalize_regular_ == "rawsubabs-sqroots-sqroots" ||
        ovalize_regular_ == "logsubabs-origins-origins" || ovalize_regular_ == "logsubabs-sqroots-sqroots" ||
        ovalize_regular_ == "expsubabs-origins-origins" || ovalize_regular_ == "expsubabs-sqroots-sqroots")
      << "illegal ovalize regular: " << ovalize_regular_ << "!"; 
  CHECK(cluster_clipmode_ == "sample-diff-based" || cluster_clipmode_ == "sample-norm-based" ||
        cluster_clipmode_ == "blobal-diff-based" || cluster_clipmode_ == "blobal-norm-based" ||
        cluster_clipmode_ == "unclipped")
      << "illegal cluster clipmode: " << cluster_clipmode_ << "!";
  CHECK(scatter_clipmode_ == "sample-diff-based" || scatter_clipmode_ == "sample-norm-based" ||
        scatter_clipmode_ == "blobal-diff-based" || scatter_clipmode_ == "blobal-norm-based" ||
        scatter_clipmode_ == "unclipped")
      << "illegal scatter clipmode: " << scatter_clipmode_ << "!";
  CHECK(topdiff_clipmode_ == "sample-norm-based" || topdiff_clipmode_ == "blobal-norm-based" ||
        topdiff_clipmode_ == "unclipped")
      << "illegal topdiff clipmode: " << topdiff_clipmode_ << "!";
  CHECK(ovalize_clipmode_ == "sample-diff-based" || ovalize_clipmode_ == "sample-norm-based" ||
        ovalize_clipmode_ == "blobal-diff-based" || ovalize_clipmode_ == "blobal-norm-based" ||
        ovalize_clipmode_ == "unclipped")
      << "illegal ovalize clipmode: " << ovalize_clipmode_ << "!";
  CHECK(outputs_activate_[0] != -1 ||
       (cluster_clipmode_ != "sample-diff-based" && cluster_clipmode_ != "blobal-diff-based"))
      << "when replica activate: false, illegal cluster clipmode: " << cluster_clipmode_ << "!";
  CHECK(outputs_activate_[0] != -1 ||
       (scatter_clipmode_ != "sample-diff-based" && scatter_clipmode_ != "blobal-diff-based"))
      << "when replica activate: false, illegal scatter clipmode: " << scatter_clipmode_ << "!";
  CHECK(outputs_activate_[0] != -1 || topdiff_clipprop_ == Dtype(0))
      << "when replica activate: false, topdiff clipprop should be set zero!";
  CHECK(outputs_activate_[0] != -1)
      << "Because of some reasons, we now only set replica activate: true!";
  CHECK(0 < label_nmax_)
      << "illegal label nmax: " << label_nmax_ << "!";
  if (this->blobs_.size()) {
    CHECK_EQ(2, this->blobs_.size()) << "Incorrect number of parameter blobs.";
    vector<int> numidx_shape(2);
    vector<int> avgsum_shape(3);
    numidx_shape[0] = label_numb_;
    numidx_shape[1] = label_nmax_;
    avgsum_shape[0] = label_numb_;
    avgsum_shape[1] = label_nmax_;
    avgsum_shape[2] = inner_numb_;
    if (numidx_shape != this->blobs_[0]->shape()) {
      Blob<Dtype> numidx_shaped_blob(numidx_shape);
      LOG(FATAL) << "Incorrect numidx shape: expected shape "
        << numidx_shaped_blob.shape_string() << "; instead, shape was "
        << this->blobs_[0]->shape_string();
    }
    if (avgsum_shape != this->blobs_[1]->shape()) {
      Blob<Dtype> avgsum_shaped_blob(avgsum_shape);
      LOG(FATAL) << "Incorrect avgsum shape: expected shape "
        << avgsum_shaped_blob.shape_string() << "; instead, shape was "
        << this->blobs_[1]->shape_string();
    }
    LOG(INFO) << "Skipping parameter initialization";
  }
  else {
    vector<int> numidx_shape(2);
    vector<int> avgsum_shape(3);
    numidx_shape[0] = label_numb_;
    numidx_shape[1] = label_nmax_;
    avgsum_shape[0] = label_numb_;
    avgsum_shape[1] = label_nmax_;
    avgsum_shape[2] = inner_numb_;
    this->blobs_.resize(2);
    this->blobs_[0].reset(new Blob<Dtype>(numidx_shape)); //data: num & diff: idx
    this->blobs_[1].reset(new Blob<Dtype>(avgsum_shape)); //data: avg & diff: sum
  }
  this->param_propagate_down_.resize(this->blobs_.size(), false);
  SyncMessenger::AddListener(
    "Any", "Any", "Solver", "Any", "SOLVER_ITER_CHANGED", 1, 0, 1,
    new SolverIterChangedHandlerForHomoMeanLossLayer<Dtype>(this)
  );
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  if (outputs_activate_[0] != -1) {
    top[outputs_activate_[0]]->ReshapeLike(*bottom[0]);
    top[outputs_activate_[0]]->ShareData(*bottom[0]);
  }
  if (outputs_activate_[1] != -1) {
    vector<int> topper_shape(3);
    topper_shape[0] = outer_numb_;
    topper_shape[1] = label_nmax_;
    topper_shape[2] = label_numb_;
    top[outputs_activate_[1]]->Reshape(topper_shape);
  }
  if (outputs_activate_[2] != -1) {
    vector<int> topper_shape(0);
    top[outputs_activate_[2]]->Reshape(topper_shape);
  }
  if (outputs_activate_[3] != -1) {
    vector<int> topper_shape(0);
    top[outputs_activate_[3]]->Reshape(topper_shape);
  }
  if (outputs_activate_[4] != -1) {
    vector<int> topper_shape(2);
    topper_shape[0] = label_numb_;
    topper_shape[1] = label_nmax_;
    top[outputs_activate_[4]]->Reshape(topper_shape);
  }
  if (outputs_activate_[5] != -1) {
    vector<int> topper_shape(2);
    topper_shape[0] = label_numb_;
    topper_shape[1] = label_nmax_;
    top[outputs_activate_[5]]->Reshape(topper_shape);
  }
  if (outputs_activate_[6] != -1) {
    vector<int> topper_shape(0);
    top[outputs_activate_[6]]->Reshape(topper_shape);
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::SolverIterChangedHandle(const void* message) {
  solver_iter_ = *(static_cast<const int*>(message));
  HomoMeanLossParameter homo_mean_loss_param = this->layer_param_.homo_mean_loss_param();
  if (!homo_mean_loss_param.has_cluster_clipprop()) {
    Dtype height   = cluster_upprprop_ - cluster_lowrprop_;
    Dtype progress = std::min(Dtype(1), solver_iter_ / cluster_feedsize_);
    cluster_clipprop_ = cluster_tradeoff_ * log(cluster_feedrate_ * progress * (exp(Dtype(1)) - 1) + 1) * height + cluster_lowrprop_;
    cluster_clipprop_ += (1 - cluster_tradeoff_) * (2 / (1 + exp(-cluster_feedrate_ * progress)) - 1) * height + cluster_lowrprop_;
    cluster_clipprop_ = std::max(Dtype(1e-7), cluster_clipprop_ / ClusterNormalizer());
  }
  if (!homo_mean_loss_param.has_scatter_clipprop()) {
    Dtype height   = scatter_upprprop_ - scatter_lowrprop_;
    Dtype progress = std::min(Dtype(1), solver_iter_ / scatter_feedsize_);
    scatter_clipprop_ = scatter_tradeoff_ * log(scatter_feedrate_ * progress * (exp(Dtype(1)) - 1) + 1) * height + scatter_lowrprop_;
    scatter_clipprop_ += (1 - scatter_tradeoff_) * (2 / (1 + exp(-scatter_feedrate_ * progress)) - 1) * height + scatter_lowrprop_;
    scatter_clipprop_ = std::max(Dtype(1e-7), scatter_clipprop_ / ScatterNormalizer());
  }
  if (!homo_mean_loss_param.has_topdiff_clipprop()) {
    Dtype height   = topdiff_upprprop_ - topdiff_lowrprop_;
    Dtype progress = std::min(Dtype(1), solver_iter_ / topdiff_feedsize_);
    topdiff_clipprop_ = topdiff_tradeoff_ * log(topdiff_feedrate_ * progress * (exp(Dtype(1)) - 1) + 1) * height + topdiff_lowrprop_;
    topdiff_clipprop_ += (1 - topdiff_tradeoff_) * (2 / (1 + exp(-topdiff_feedrate_ * progress)) - 1) * height + topdiff_lowrprop_;
  }
  if (!homo_mean_loss_param.has_ovalize_clipprop()) {
    Dtype height   = ovalize_upprprop_ - ovalize_lowrprop_;
    Dtype progress = std::min(Dtype(1), solver_iter_ / ovalize_feedsize_);
    ovalize_clipprop_ = ovalize_tradeoff_ * log(ovalize_feedrate_ * progress * (exp(Dtype(1)) - 1) + 1) * height + ovalize_lowrprop_;
    ovalize_clipprop_ += (1 - ovalize_tradeoff_) * (2 / (1 + exp(-ovalize_feedrate_ * progress)) - 1) * height + ovalize_lowrprop_;
    ovalize_clipprop_ = std::max(Dtype(1e-7), ovalize_clipprop_ / OvalizeNormalizer());
  }
}

template <typename Dtype>
Dtype HomoMeanLossLayer<Dtype>::ClusterNormalizer() {
  Dtype normalizer;
  switch (cluster_normalization_) {
    case HomoMeanLossParameter_NormalizationMode_FULL:
      normalizer = Dtype(outer_numb_ * inner_numb_);
      break;
    case HomoMeanLossParameter_NormalizationMode_BATCH_SIZE:
      normalizer = Dtype(outer_numb_);
      break;
    case HomoMeanLossParameter_NormalizationMode_NONE:
      normalizer = Dtype(1);
      break;
    default:
      LOG(FATAL) << "Unknown cluster normalization mode: "
          << HomoMeanLossParameter_NormalizationMode_Name(cluster_normalization_);
  }
  return std::max(Dtype(1.0), normalizer);
}

template <typename Dtype>
Dtype HomoMeanLossLayer<Dtype>::ScatterNormalizer() {
  Dtype normalizer;
  switch (scatter_normalization_) {
    case HomoMeanLossParameter_NormalizationMode_FULL:
      normalizer = Dtype(outer_numb_ * inner_numb_);
      break;
    case HomoMeanLossParameter_NormalizationMode_BATCH_SIZE:
      normalizer = Dtype(outer_numb_);
      break;
    case HomoMeanLossParameter_NormalizationMode_NONE:
      normalizer = Dtype(1);
      break;
    default:
      LOG(FATAL) << "Unknown scatter normalization mode: "
          << HomoMeanLossParameter_NormalizationMode_Name(scatter_normalization_);
  }
  return std::max(Dtype(1.0), normalizer);
}

template <typename Dtype>
Dtype HomoMeanLossLayer<Dtype>::OvalizeNormalizer() {
  Dtype normalizer;
  switch (ovalize_normalization_) {
    case HomoMeanLossParameter_NormalizationMode_FULL:
      normalizer = Dtype(match_numb_ * inner_numb_);
      break;
    case HomoMeanLossParameter_NormalizationMode_BATCH_SIZE:
      normalizer = Dtype(match_numb_);
      break;
    case HomoMeanLossParameter_NormalizationMode_NONE:
      normalizer = Dtype(1);
      break;
    default:
      LOG(FATAL) << "Unknown ovalize normalization mode: "
          << HomoMeanLossParameter_NormalizationMode_Name(ovalize_normalization_);
  }
  return std::max(Dtype(1.0), normalizer);
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ClusterForward_cpu(const vector<Blob<Dtype>*>& top) {
  Dtype* topper_datum = top[outputs_activate_[2]]->mutable_cpu_data();
  Dtype* clustr_datum = clustr_blob_.mutable_cpu_data();
  Dtype* clustr_diffs = clustr_blob_.mutable_cpu_diff();
  const Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  const Dtype* middle_diffs = middle_blob_.mutable_cpu_diff();
  for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
          Dtype* clustr_datpt = clustr_datum + inner_index;
          Dtype* clustr_difpt = clustr_diffs + inner_index;
    const Dtype* middle_datpt = middle_datum + inner_index;
    const Dtype* middle_difpt = middle_diffs + inner_index;
          Dtype  reduce_datum = 0, reduce_count = 0;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
      if (static_cast<int>(*middle_difpt) > 0) {
        reduce_datum += *middle_datpt / *middle_difpt;
        reduce_count += 1;
      }
      middle_datpt += inner_numb_;
      middle_difpt += inner_numb_;
    }
    if (static_cast<int>(reduce_count) > 0) {
       reduce_count += *clustr_difpt;
      *clustr_datpt *= *clustr_difpt / reduce_count;
      *clustr_datpt +=  reduce_datum / reduce_count;
      *clustr_difpt  =  reduce_count;
    }
  }
  *topper_datum  = caffe_cpu_asum(inner_numb_, clustr_datum);
  *topper_datum /= inner_numb_;
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ScatterForward_cpu(const vector<Blob<Dtype>*>& top) {
  Dtype* topper_datum = top[outputs_activate_[3]]->mutable_cpu_data();
  Dtype* scattr_datum = scattr_blob_.mutable_cpu_data();
  Dtype* scattr_diffs = scattr_blob_.mutable_cpu_diff();
  const Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  const Dtype* middle_diffs = middle_blob_.mutable_cpu_diff();
  for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
          Dtype* scattr_datpt = scattr_datum + inner_index;
          Dtype* scattr_difpt = scattr_diffs + inner_index;
    const Dtype* middle_datpt = middle_datum + inner_index;
    const Dtype* middle_difpt = middle_diffs + inner_index;
          Dtype  reduce_datum = 0, reduce_count = 0;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
      if (static_cast<int>(*middle_difpt) > 0) {
        reduce_datum += *middle_datpt / *middle_difpt;
        reduce_count += 1;
      }
      middle_datpt += inner_numb_;
      middle_difpt += inner_numb_;
    }
    if (static_cast<int>(reduce_count) > 0) {
       reduce_count += *scattr_difpt;
      *scattr_datpt *= *scattr_difpt / reduce_count;
      *scattr_datpt +=  reduce_datum / reduce_count;
      *scattr_difpt  =  reduce_count;
    }
  }
  *topper_datum  = caffe_cpu_asum(inner_numb_, scattr_datum);
  *topper_datum /= inner_numb_;
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ClusupdForward_cpu(const vector<Blob<Dtype>*>& top) {
  Dtype* topper_datum = top[outputs_activate_[4]]->mutable_cpu_data();
  Dtype* clusup_datum = clusup_blob_.mutable_cpu_data();
  Dtype* clusup_diffs = clusup_blob_.mutable_cpu_diff();
  const Dtype* medium_datum = medium_blob_.mutable_cpu_data();
  const Dtype* medium_diffs = medium_blob_.mutable_cpu_diff();
  const int round_count = label_numb_ * label_nmax_;
  for (int round_index = 0; round_index < round_count; ++round_index) {
          Dtype* topper_datpt = topper_datum + round_index;
          Dtype* clusup_datpt = clusup_datum + round_index * inner_numb_;
          Dtype* clusup_difpt = clusup_diffs + round_index * inner_numb_;
    const Dtype* medium_datpt = medium_datum + round_index * inner_numb_;
    const Dtype* medium_difpt = medium_diffs + round_index * inner_numb_;
                *topper_datpt = 0;
    for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
      if (static_cast<int>(*medium_difpt) > 0) {
        Dtype reduce_count = *clusup_difpt + *medium_difpt;
        *clusup_datpt *= *clusup_difpt / reduce_count;
        *clusup_datpt += *medium_datpt / reduce_count;
        *clusup_difpt  =  reduce_count;
        *topper_datpt += *clusup_datpt;
      }
      ++medium_datpt; ++medium_difpt;
      ++clusup_datpt; ++clusup_difpt;
    }
    *topper_datpt /= inner_numb_;
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ScatupdForward_cpu(const vector<Blob<Dtype>*>& top) {
  Dtype* topper_datum = top[outputs_activate_[5]]->mutable_cpu_data();
  Dtype* scatup_datum = scatup_blob_.mutable_cpu_data();
  Dtype* scatup_diffs = scatup_blob_.mutable_cpu_diff();
  const Dtype* medium_datum = medium_blob_.mutable_cpu_data();
  const Dtype* medium_diffs = medium_blob_.mutable_cpu_diff();
  const int round_count = label_numb_ * label_nmax_;
  for (int round_index = 0; round_index < round_count; ++round_index) {
          Dtype* topper_datpt = topper_datum + round_index;
          Dtype* scatup_datpt = scatup_datum + round_index * inner_numb_;
          Dtype* scatup_difpt = scatup_diffs + round_index * inner_numb_;
    const Dtype* medium_datpt = medium_datum + round_index * inner_numb_;
    const Dtype* medium_difpt = medium_diffs + round_index * inner_numb_;
                *topper_datpt = 0;
    for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
      if (static_cast<int>(*medium_difpt) > 0) {
        Dtype reduce_count = *scatup_difpt + *medium_difpt;
        *scatup_datpt *= *scatup_difpt / reduce_count;
        *scatup_datpt += *medium_datpt / reduce_count;
        *scatup_difpt  =  reduce_count;
        *topper_datpt += *scatup_datpt;
      }
      ++medium_datpt; ++medium_difpt;
      ++scatup_datpt; ++scatup_difpt;
    }
    *topper_datpt /= inner_numb_;
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::OvalizeForward_cpu(const vector<Blob<Dtype>*>& top) {
  Dtype* topper_datum = top[outputs_activate_[6]]->mutable_cpu_data();
  Dtype* ovaliz_datum = ovaliz_blob_.mutable_cpu_data();
  Dtype* ovaliz_diffs = ovaliz_blob_.mutable_cpu_diff();
  const Dtype* caches_datum = caches_blob_.mutable_cpu_data();
  const Dtype* caches_diffs = caches_blob_.mutable_cpu_diff();
  for (int label_index = 0; label_index < label_numb_; ++label_index) {
          Dtype* ovaliz_datpt = ovaliz_datum + label_index;
          Dtype* ovaliz_difpt = ovaliz_diffs + label_index;
    const Dtype* caches_datpt = caches_datum + label_index;
    const Dtype* caches_difpt = caches_diffs + label_index;
          Dtype  reduce_datum = 0, reduce_count = 0;
    for (int match_index = 0; match_index < match_numb_; ++match_index) {
      if (static_cast<int>(*caches_difpt) > 0) {
        reduce_datum += *caches_datpt / *caches_difpt;
        reduce_count += 1;
      }
      caches_datpt += label_numb_;
      caches_difpt += label_numb_;
    }
    if (static_cast<int>(reduce_count) > 0) {
       reduce_count += *ovaliz_difpt;
      *ovaliz_datpt *= *ovaliz_difpt / reduce_count;
      *ovaliz_datpt +=  reduce_datum / reduce_count;
      *ovaliz_difpt  =  reduce_count;
    }
  }
  *topper_datum  = caffe_cpu_asum(label_numb_, ovaliz_datum);
  *topper_datum /= label_numb_;
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::OvalizeMatcher_cpu(const vector<Blob<Dtype>*>& bottom) {
  const bool ovs2s_check = (ovals2s_01stprop_ != Dtype(0) || ovals2s_02ndprop_ != Dtype(0));
  const bool ovt2t_check = (ovalt2t_01stprop_ != Dtype(0) || ovalt2t_02ndprop_ != Dtype(0));
  const bool ovs2t_check = (ovals2t_01stprop_ != Dtype(0) || ovals2t_02ndprop_ != Dtype(0));
  const int otask_count = ovs2s_check + ovt2t_check + ovs2t_check * 2;
  const int ovs2s_count = ovs2s_check ? (match_numb_ / otask_count) : 0;
  const int ovt2t_count = ovt2t_check ? (match_numb_ / otask_count) : 0;
  const int ovs2t_count = ovs2t_check ? (match_numb_ / otask_count) : 0;
  const int ovall_count = ovs2s_count + ovt2t_count + ovs2t_count * 2;
  match_numb_ = ovall_count ? ovall_count : match_numb_;
  vector<int> mapidx_shape(2);
  vector<int> mapair_shape(2);
  mapidx_shape[0] = outer_numb_;
  mapidx_shape[1] = label_numb_;
  mapair_shape[0] = match_numb_;
  mapair_shape[1] = label_numb_;
  mapidx_blob_.Reshape(mapidx_shape);
  mapair_blob_.Reshape(mapair_shape);
  maprop_blob_.Reshape(mapair_shape);
  int* mapidx_datum = mapidx_blob_.mutable_cpu_data();
  int* mapidx_diffs = mapidx_blob_.mutable_cpu_diff();
  int* mapair_datum = mapair_blob_.mutable_cpu_data();
  int* mapair_diffs = mapair_blob_.mutable_cpu_diff();
  Dtype* maprop_datum = maprop_blob_.mutable_cpu_data();
  Dtype* maprop_diffs = maprop_blob_.mutable_cpu_diff();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  for (int label_index = 0; label_index < label_numb_; ++label_index) {
    *(mapair_datum + label_index) = rand();
    *(mapair_diffs + label_index) = rand();
  }
  for (int label_index = 0; label_index < label_numb_; ++label_index) {
    int  mapidx_datnc = 0, mapidx_difnc = 0;
    int* mapidx_datpt = mapidx_datum + label_index;
    int* mapidx_difpt = mapidx_diffs + label_index;
    const Dtype* bottom_labpt = bottom_label + label_index;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
      if (static_cast<int>(*bottom_labpt) == target_label_) {
        *mapidx_difpt  = outer_index;
         mapidx_difpt += label_numb_;
         mapidx_difnc += 1;
      } else if (static_cast<int>(*bottom_labpt) != ignore_label_) {
        *mapidx_datpt  = outer_index;
         mapidx_datpt += label_numb_;
         mapidx_datnc += 1;
      }
      bottom_labpt += label_numb_;
    }
    int*   mapair_datpt = mapair_datum + label_index;
    int*   mapair_difpt = mapair_diffs + label_index;
    Dtype* maprop_datpt = maprop_datum + label_index;
    Dtype* maprop_difpt = maprop_diffs + label_index;
    if (ovall_count == 0 || mapidx_datnc < 2 || mapidx_difnc < 2) {
      for (int match_index = 0; match_index < match_numb_; ++match_index) {
        *mapair_datpt = *mapair_difpt = -1;
        *maprop_datpt = *maprop_difpt = +0;
        mapair_datpt += label_numb_;
        mapair_difpt += label_numb_;
        maprop_datpt += label_numb_;
        maprop_difpt += label_numb_;
      }
    } else {
      // shuffling mapidx_datum and mapidx_diffs, then filling mapair_datum
      if (ovall_count) srand(*mapair_datpt);
      if (ovs2s_count || ovs2t_count) {
        for (int mapidx_datid = mapidx_datnc - 1; mapidx_datid > 0; --mapidx_datid) {
          int* mapidx_srcpt = mapidx_datpt - label_numb_ * (mapidx_datnc - mapidx_datid);
          int* mapidx_trgpt = mapidx_datpt - label_numb_ * (mapidx_datnc - rand() % (mapidx_datid + 1));
          int  mapidx_value = *mapidx_srcpt;
          *mapidx_srcpt = *mapidx_trgpt;
          *mapidx_trgpt =  mapidx_value;
        }
      }
      if (ovt2t_count) {
        for (int mapidx_difid = mapidx_difnc - 1; mapidx_difid > 0; --mapidx_difid) {
          int* mapidx_srcpt = mapidx_difpt - label_numb_ * (mapidx_difnc - mapidx_difid);
          int* mapidx_trgpt = mapidx_difpt - label_numb_ * (mapidx_difnc - rand() % (mapidx_difid + 1));
          int  mapidx_value = *mapidx_srcpt;
          *mapidx_srcpt = *mapidx_trgpt;
          *mapidx_trgpt =  mapidx_value;
        }
      }
      for (int mapair_datid = 0; mapair_datid < ovs2s_count; ++mapair_datid) {
        *maprop_datpt = ovals2s_01stprop_;
        *mapair_datpt = *(mapidx_datpt - label_numb_ * (mapidx_datnc - mapair_datid));
         maprop_datpt += label_numb_;
         mapair_datpt += label_numb_;
      }
      for (int mapair_datid = 0; mapair_datid < ovt2t_count; ++mapair_datid) {
        *maprop_datpt = ovalt2t_01stprop_;
        *mapair_datpt = *(mapidx_difpt - label_numb_ * (mapidx_difnc - mapair_datid));
         maprop_datpt += label_numb_;
         mapair_datpt += label_numb_;
      }
      for (int mapair_datid = 0; mapair_datid < ovs2t_count; ++mapair_datid) {
        *maprop_datpt = ovals2t_01stprop_;
        *mapair_datpt = *(mapidx_datpt - label_numb_ * (mapidx_datnc - mapair_datid));
         maprop_datpt += label_numb_;
         mapair_datpt += label_numb_;
      }
      for (int mapair_datid = 0; mapair_datid < ovs2t_count; ++mapair_datid) {
        *maprop_datpt = ovals2t_02ndprop_;
        *mapair_datpt = *(mapidx_difpt - label_numb_ * (mapidx_difnc - mapair_datid));
         maprop_datpt += label_numb_;
         mapair_datpt += label_numb_;
      }
      // shuffling mapidx_datum and mapidx_diffs, then filling mapair_diffs
      if (ovall_count) srand(*mapair_difpt);
      if (ovs2s_count) {
        for (int mapidx_datid = mapidx_datnc - 1; mapidx_datid > 0; --mapidx_datid) {
          int* mapidx_srcpt = mapidx_datpt - label_numb_ * (mapidx_datnc - mapidx_datid);
          int* mapidx_trgpt = mapidx_datpt - label_numb_ * (mapidx_datnc - rand() % (mapidx_datid + 1));
          int  mapidx_value = *mapidx_srcpt;
          *mapidx_srcpt = *mapidx_trgpt;
          *mapidx_trgpt =  mapidx_value;
        }
      }
      if (ovt2t_count || ovs2t_count) {
        for (int mapidx_difid = mapidx_difnc - 1; mapidx_difid > 0; --mapidx_difid) {
          int* mapidx_srcpt = mapidx_difpt - label_numb_ * (mapidx_difnc - mapidx_difid);
          int* mapidx_trgpt = mapidx_difpt - label_numb_ * (mapidx_difnc - rand() % (mapidx_difid + 1));
          int  mapidx_value = *mapidx_srcpt;
          *mapidx_srcpt = *mapidx_trgpt;
          *mapidx_trgpt =  mapidx_value;
        }
      }
      for (int mapair_difid = 0; mapair_difid < ovs2s_count; ++mapair_difid) {
        *maprop_difpt = ovals2s_02ndprop_;
        *mapair_difpt = *(mapidx_datpt - label_numb_ * (mapidx_datnc - mapair_difid));
         maprop_difpt += label_numb_;
         mapair_difpt += label_numb_;
      }
      for (int mapair_difid = 0; mapair_difid < ovt2t_count; ++mapair_difid) {
        *maprop_difpt = ovalt2t_02ndprop_;
        *mapair_difpt = *(mapidx_difpt - label_numb_ * (mapidx_difnc - mapair_difid));
         maprop_difpt += label_numb_;
         mapair_difpt += label_numb_;
      }
      for (int mapair_difid = 0; mapair_difid < ovs2t_count; ++mapair_difid) {
        *maprop_difpt = ovals2t_02ndprop_;
        *mapair_difpt = *(mapidx_difpt - label_numb_ * (mapidx_difnc - mapair_difid));
         maprop_difpt += label_numb_;
         mapair_difpt += label_numb_;
      }
      for (int mapair_difid = 0; mapair_difid < ovs2t_count; ++mapair_difid) {
        *maprop_difpt = ovals2t_01stprop_;
        *mapair_difpt = *(mapidx_datpt - label_numb_ * (mapidx_datnc - mapair_difid));
         maprop_difpt += label_numb_;
         mapair_difpt += label_numb_;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ClusterTesting_cpu(const vector<Blob<Dtype>*>& bottom) {
  middle_blob_.ReshapeLike(*bottom[0]);
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  Dtype* middle_diffs = middle_blob_.mutable_cpu_diff();
  if (cluster_measure_ == "rawsubsqr-overall-average" || cluster_measure_ == "rawsubsqr-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += buffer_datum * buffer_datum;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "logsubsqr-overall-average" || cluster_measure_ == "logsubsqr-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += log(buffer_datum * buffer_datum + 1);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "expsubsqr-overall-average" || cluster_measure_ == "expsubsqr-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += 1 - exp(-buffer_datum * buffer_datum);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "rawsubabs-overall-average" || cluster_measure_ == "rawsubabs-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += abs(buffer_datum);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "logsubabs-overall-average" || cluster_measure_ == "logsubabs-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += log(abs(buffer_datum) + 1);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "expsubabs-overall-average" || cluster_measure_ == "expsubabs-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += 1 - exp(-abs(buffer_datum));
        *middle_difpt += 1;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ClusterMeasure_cpu(const vector<Blob<Dtype>*>& bottom) {
  middle_blob_.ReshapeLike(*bottom[0]);
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  Dtype* middle_diffs = middle_blob_.mutable_cpu_diff();
  if (cluster_measure_ == "rawsubsqr-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += buffer_datum * buffer_datum;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "rawsubsqr-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += buffer_datum * buffer_datum;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "logsubsqr-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += log(buffer_datum * buffer_datum + 1);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "logsubsqr-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += log(buffer_datum * buffer_datum + 1);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "expsubsqr-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += 1 - exp(-buffer_datum * buffer_datum);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "expsubsqr-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += 1 - exp(-buffer_datum * buffer_datum);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "rawsubabs-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += abs(buffer_datum);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "rawsubabs-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += abs(buffer_datum);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "logsubabs-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += log(abs(buffer_datum) + 1);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "logsubabs-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += log(abs(buffer_datum) + 1);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "expsubabs-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += 1 - exp(-abs(buffer_datum));
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_measure_ == "expsubabs-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += 1 - exp(-abs(buffer_datum));
        *middle_difpt += 1;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ScatterMeasure_cpu(const vector<Blob<Dtype>*>& bottom) {
  middle_blob_.ReshapeLike(*bottom[0]);
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  Dtype* middle_diffs = middle_blob_.mutable_cpu_diff();
  if (scatter_measure_ == "rawsubsqr-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
          *middle_datpt += buffer_datum * buffer_datum;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "rawsubsqr-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
          *middle_datpt += buffer_datum * buffer_datum;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "logsubsqr-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
          *middle_datpt += log(buffer_datum * buffer_datum + 1);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "logsubsqr-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
          *middle_datpt += log(buffer_datum * buffer_datum + 1);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "expsubsqr-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
          *middle_datpt += 1 - exp(-buffer_datum * buffer_datum);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "expsubsqr-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
          *middle_datpt += 1 - exp(-buffer_datum * buffer_datum);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "rawsubabs-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
          *middle_datpt += abs(buffer_datum);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "rawsubabs-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
          *middle_datpt += abs(buffer_datum);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "logsubabs-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
          *middle_datpt += log(abs(buffer_datum) + 1);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "logsubabs-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
          *middle_datpt += log(abs(buffer_datum) + 1);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "expsubabs-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
          *middle_datpt += 1 - exp(-abs(buffer_datum));
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_measure_ == "expsubabs-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
          *middle_datpt += 1 - exp(-abs(buffer_datum));
          *middle_difpt += 1;
        }
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ClusupdTesting_cpu(const vector<Blob<Dtype>*>& bottom) {
  medium_blob_.ReshapeLike(*this->blobs_[1]);
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* medium_datum = medium_blob_.mutable_cpu_data();
  Dtype* medium_diffs = medium_blob_.mutable_cpu_diff();
  if (clusupd_measure_ == "rawsubsqr-overall-average" || clusupd_measure_ == "rawsubsqr-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += buffer_datum * buffer_datum;
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "logsubsqr-overall-average" || clusupd_measure_ == "logsubsqr-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += log(buffer_datum * buffer_datum + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "expsubsqr-overall-average" || clusupd_measure_ == "expsubsqr-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += 1 - exp(-buffer_datum * buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "rawsubabs-overall-average" || clusupd_measure_ == "rawsubabs-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += abs(buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "logsubabs-overall-average" || clusupd_measure_ == "logsubabs-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += log(abs(buffer_datum) + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "expsubabs-overall-average" || clusupd_measure_ == "expsubabs-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += 1 - exp(-abs(buffer_datum));
        *medium_difpt += 1;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ClusupdMeasure_cpu(const vector<Blob<Dtype>*>& bottom) {
  medium_blob_.ReshapeLike(*this->blobs_[1]);
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* medium_datum = medium_blob_.mutable_cpu_data();
  Dtype* medium_diffs = medium_blob_.mutable_cpu_diff();
  if (clusupd_measure_ == "rawsubsqr-overall-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += buffer_datum * buffer_datum;
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "rawsubsqr-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *medium_datpt += buffer_datum * buffer_datum;
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "logsubsqr-overall-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += log(buffer_datum * buffer_datum + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "logsubsqr-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *medium_datpt += log(buffer_datum * buffer_datum + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "expsubsqr-overall-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += 1 - exp(-buffer_datum * buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "expsubsqr-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *medium_datpt += 1 - exp(-buffer_datum * buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "rawsubabs-overall-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += abs(buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "rawsubabs-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *medium_datpt += abs(buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "logsubabs-overall-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += log(abs(buffer_datum) + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "logsubabs-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *medium_datpt += log(abs(buffer_datum) + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "expsubabs-overall-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += 1 - exp(-abs(buffer_datum));
        *medium_difpt += 1;
      }
    }
  }
  else if (clusupd_measure_ == "expsubabs-nonself-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *medium_datpt += 1 - exp(-abs(buffer_datum));
        *medium_difpt += 1;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ScatupdMeasure_cpu(const vector<Blob<Dtype>*>& bottom) {
  medium_blob_.ReshapeLike(*this->blobs_[1]);
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* medium_datum = medium_blob_.mutable_cpu_data();
  Dtype* medium_diffs = medium_blob_.mutable_cpu_diff();
  if (scatupd_measure_ == "rawsubsqr-samples-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += buffer_datum * buffer_datum;
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "rawsubsqr-average-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
        *medium_datpt += buffer_datum * buffer_datum;
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "logsubsqr-samples-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += log(buffer_datum * buffer_datum + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "logsubsqr-average-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
        *medium_datpt += log(buffer_datum * buffer_datum + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "expsubsqr-samples-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += 1 - exp(-buffer_datum * buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "expsubsqr-average-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
        *medium_datpt += 1 - exp(-buffer_datum * buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "rawsubabs-samples-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += abs(buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "rawsubabs-average-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
        *medium_datpt += abs(buffer_datum);
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "logsubabs-samples-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += log(abs(buffer_datum) + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "logsubabs-average-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
        *medium_datpt += log(abs(buffer_datum) + 1);
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "expsubabs-samples-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *medium_datpt += 1 - exp(-abs(buffer_datum));
        *medium_difpt += 1;
      }
    }
  }
  else if (scatupd_measure_ == "expsubabs-average-average") {
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
        const int botlab_shift = outer_index * label_numb_ + label_index;
        const Dtype* bottom_labpt = bottom_label + botlab_shift;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift;
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift;
        if (static_cast<int>(*bottom_labpt) == labmx_index) continue;
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        if (static_cast<int>(*numidx_datit) < 1) continue;
        Dtype buffer_datum = *avgsum_datit - *avgsum_datpt;
        *medium_datpt += 1 - exp(-abs(buffer_datum));
        *medium_difpt += 1;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::OdotterTesting_cpu(const vector<Blob<Dtype>*>& bottom) {
  vector<int> medial_shape(3);
  medial_shape[0] = match_numb_;
  medial_shape[1] = label_numb_;
  medial_shape[2] = label_nmax_;
  medial_blob_.Reshape(medial_shape);
  const int* mapair_datum = mapair_blob_.cpu_data();
  const int* mapair_diffs = mapair_blob_.cpu_diff();
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* medial_datum = medial_blob_.mutable_cpu_data();
  Dtype* medial_diffs = medial_blob_.mutable_cpu_diff();
  if (odotter_measure_ == "rawsubsqr-overall-average" || odotter_measure_ == "rawsubsqr-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += srcbuf_datum * srcbuf_datum;
        *medial_difpt += trgbuf_datum * trgbuf_datum;
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "logsubsqr-overall-average" || odotter_measure_ == "logsubsqr-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += log(srcbuf_datum * srcbuf_datum + 1);
        *medial_difpt += log(trgbuf_datum * trgbuf_datum + 1);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "expsubsqr-overall-average" || odotter_measure_ == "expsubsqr-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += 1 - exp(-srcbuf_datum * srcbuf_datum);
        *medial_difpt += 1 - exp(-trgbuf_datum * trgbuf_datum);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "rawsubabs-overall-average" || odotter_measure_ == "rawsubabs-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += abs(srcbuf_datum);
        *medial_difpt += abs(trgbuf_datum);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "logsubabs-overall-average" || odotter_measure_ == "logsubabs-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += log(abs(srcbuf_datum) + 1);
        *medial_difpt += log(abs(trgbuf_datum) + 1);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "expsubabs-overall-average" || odotter_measure_ == "expsubabs-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += 1 - exp(-abs(srcbuf_datum));
        *medial_difpt += 1 - exp(-abs(trgbuf_datum));
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::OdotterMeasure_cpu(const vector<Blob<Dtype>*>& bottom) {
  vector<int> medial_shape(3);
  medial_shape[0] = match_numb_;
  medial_shape[1] = label_numb_;
  medial_shape[2] = label_nmax_;
  medial_blob_.Reshape(medial_shape);
  const int* mapair_datum = mapair_blob_.cpu_data();
  const int* mapair_diffs = mapair_blob_.cpu_diff();
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* medial_datum = medial_blob_.mutable_cpu_data();
  Dtype* medial_diffs = medial_blob_.mutable_cpu_diff();
  if (odotter_measure_ == "rawsubsqr-overall-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += srcbuf_datum * srcbuf_datum;
        *medial_difpt += trgbuf_datum * trgbuf_datum;
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "rawsubsqr-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      const Dtype* srcbot_labpt = bottom_label + *mapair_datpt * label_numb_ + label_index;
      const Dtype* trgbot_labpt = bottom_label + *mapair_difpt * label_numb_ + label_index;
      const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
      const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1 + srclab_check) continue;
      if (static_cast<int>(*numidx_datpt) < 1 + trglab_check) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srcbuf_error * srclab_check;
        const Dtype avgsum_volum = *avgsum_datpt + trgbuf_error * trglab_check;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        *medial_datpt += srcbuf_datum * srcbuf_datum;
        *medial_difpt += trgbuf_datum * trgbuf_datum;
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "logsubsqr-overall-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += log(srcbuf_datum * srcbuf_datum + 1);
        *medial_difpt += log(trgbuf_datum * trgbuf_datum + 1);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "logsubsqr-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      const Dtype* srcbot_labpt = bottom_label + *mapair_datpt * label_numb_ + label_index;
      const Dtype* trgbot_labpt = bottom_label + *mapair_difpt * label_numb_ + label_index;
      const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
      const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1 + srclab_check) continue;
      if (static_cast<int>(*numidx_datpt) < 1 + trglab_check) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srcbuf_error * srclab_check;
        const Dtype avgsum_volum = *avgsum_datpt + trgbuf_error * trglab_check;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        *medial_datpt += log(srcbuf_datum * srcbuf_datum + 1);
        *medial_difpt += log(trgbuf_datum * trgbuf_datum + 1);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "expsubsqr-overall-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += 1 - exp(-srcbuf_datum * srcbuf_datum);
        *medial_difpt += 1 - exp(-trgbuf_datum * trgbuf_datum);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "expsubsqr-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      const Dtype* srcbot_labpt = bottom_label + *mapair_datpt * label_numb_ + label_index;
      const Dtype* trgbot_labpt = bottom_label + *mapair_difpt * label_numb_ + label_index;
      const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
      const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1 + srclab_check) continue;
      if (static_cast<int>(*numidx_datpt) < 1 + trglab_check) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srcbuf_error * srclab_check;
        const Dtype avgsum_volum = *avgsum_datpt + trgbuf_error * trglab_check;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        *medial_datpt += 1 - exp(-srcbuf_datum * srcbuf_datum);
        *medial_difpt += 1 - exp(-trgbuf_datum * trgbuf_datum);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "rawsubabs-overall-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += abs(srcbuf_datum);
        *medial_difpt += abs(trgbuf_datum);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "rawsubabs-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      const Dtype* srcbot_labpt = bottom_label + *mapair_datpt * label_numb_ + label_index;
      const Dtype* trgbot_labpt = bottom_label + *mapair_difpt * label_numb_ + label_index;
      const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
      const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1 + srclab_check) continue;
      if (static_cast<int>(*numidx_datpt) < 1 + trglab_check) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srcbuf_error * srclab_check;
        const Dtype avgsum_volum = *avgsum_datpt + trgbuf_error * trglab_check;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        *medial_datpt += abs(srcbuf_datum);
        *medial_difpt += abs(trgbuf_datum);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "logsubabs-overall-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += log(abs(srcbuf_datum) + 1);
        *medial_difpt += log(abs(trgbuf_datum) + 1);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "logsubabs-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      const Dtype* srcbot_labpt = bottom_label + *mapair_datpt * label_numb_ + label_index;
      const Dtype* trgbot_labpt = bottom_label + *mapair_difpt * label_numb_ + label_index;
      const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
      const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1 + srclab_check) continue;
      if (static_cast<int>(*numidx_datpt) < 1 + trglab_check) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srcbuf_error * srclab_check;
        const Dtype avgsum_volum = *avgsum_datpt + trgbuf_error * trglab_check;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        *medial_datpt += log(abs(srcbuf_datum) + 1);
        *medial_difpt += log(abs(trgbuf_datum) + 1);
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "expsubabs-overall-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        *medial_datpt += 1 - exp(-abs(srcbuf_datum));
        *medial_difpt += 1 - exp(-abs(trgbuf_datum));
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
  else if (odotter_measure_ == "expsubabs-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * label_nmax_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int labmx_index = round_index % label_nmax_;
      const int label_index = round_index / label_nmax_ % label_numb_;
      const int match_index = round_index / label_nmax_ / label_numb_;
      const int  mapair_shift = match_index * label_numb_ + label_index;
      const int  numidx_shift = label_index * label_nmax_ + labmx_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const Dtype* numidx_datpt = numidx_datum +  numidx_shift;
      const Dtype* avgsum_datpt = avgsum_datum +  numidx_shift * inner_numb_;
      const Dtype* srcbot_datpt = bottom_datum + *mapair_datpt * inner_numb_;
      const Dtype* trgbot_datpt = bottom_datum + *mapair_difpt * inner_numb_;
      const Dtype* srcbot_labpt = bottom_label + *mapair_datpt * label_numb_ + label_index;
      const Dtype* trgbot_labpt = bottom_label + *mapair_difpt * label_numb_ + label_index;
      const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
      const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
      Dtype* medial_datpt = medial_datum + round_index;
      Dtype* medial_difpt = medial_diffs + round_index;
            *medial_datpt = *medial_difpt = -1;
      if (static_cast<int>(*numidx_datpt) < 1 + srclab_check) continue;
      if (static_cast<int>(*numidx_datpt) < 1 + trglab_check) continue;
      if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
            *medial_datpt = *medial_difpt = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srcbuf_error * srclab_check;
        const Dtype avgsum_volum = *avgsum_datpt + trgbuf_error * trglab_check;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        *medial_datpt += 1 - exp(-abs(srcbuf_datum));
        *medial_difpt += 1 - exp(-abs(trgbuf_datum));
        ++srcbot_datpt; ++trgbot_datpt;
        ++avgsum_datpt;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::OvalizeMeasure_cpu(const vector<Blob<Dtype>*>& bottom) {
  vector<int> caches_shape(2);
  caches_shape[0] = match_numb_;
  caches_shape[1] = label_numb_;
  caches_blob_.Reshape(caches_shape);
  const Dtype* medial_datum = medial_blob_.cpu_data();
  const Dtype* medial_diffs = medial_blob_.cpu_diff();
  Dtype* caches_datum = caches_blob_.mutable_cpu_data();
  Dtype* caches_diffs = caches_blob_.mutable_cpu_diff();
  if (ovalize_measure_ == "rawsubsqr-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += *medial_datpt - *medial_difpt;
        *caches_difpt += 1;
      }
      *caches_datpt = *caches_datpt * *caches_datpt;
    }
  }
  else if (ovalize_measure_ == "rawsubsqr-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        *caches_difpt += 1;
      }
      *caches_datpt = *caches_datpt * *caches_datpt;
    }
  }
  else if (ovalize_measure_ == "logsubsqr-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += *medial_datpt - *medial_difpt;
        *caches_difpt += 1;
      }
      *caches_datpt = log(*caches_datpt * *caches_datpt + 1);
    }
  }
  else if (ovalize_measure_ == "logsubsqr-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        *caches_difpt += 1;
      }
      *caches_datpt = log(*caches_datpt * *caches_datpt + 1);
    }
  }
  else if (ovalize_measure_ == "expsubsqr-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += *medial_datpt - *medial_difpt;
        *caches_difpt += 1;
      }
      *caches_datpt = 1 - exp(-*caches_datpt * *caches_datpt);
    }
  }
  else if (ovalize_measure_ == "expsubsqr-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        *caches_difpt += 1;
      }
      *caches_datpt = 1 - exp(-*caches_datpt * *caches_datpt);
    }
  }
  else if (ovalize_measure_ == "rawsubabs-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += *medial_datpt - *medial_difpt;
        *caches_difpt += 1;
      }
      *caches_datpt = abs(*caches_datpt);
    }
  }
  else if (ovalize_measure_ == "rawsubabs-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        *caches_difpt += 1;
      }
      *caches_datpt = abs(*caches_datpt);
    }
  }
  else if (ovalize_measure_ == "logsubabs-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += *medial_datpt - *medial_difpt;
        *caches_difpt += 1;
      }
      *caches_datpt = log(abs(*caches_datpt) + 1);
    }
  }
  else if (ovalize_measure_ == "logsubabs-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        *caches_difpt += 1;
      }
      *caches_datpt = log(abs(*caches_datpt) + 1);
    }
  }
  else if (ovalize_measure_ == "expsubabs-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += *medial_datpt - *medial_difpt;
        *caches_difpt += 1;
      }
      *caches_datpt = 1 - exp(-abs(*caches_datpt));
    }
  }
  else if (ovalize_measure_ == "expsubabs-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype* caches_datpt = caches_datum + round_index;
      Dtype* caches_difpt = caches_diffs + round_index;
            *caches_datpt = *caches_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        *caches_datpt += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        *caches_difpt += 1;
      }
      *caches_datpt = 1 - exp(-abs(*caches_datpt));
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ClusterRegular_cpu(const vector<Blob<Dtype>*>& bottom) {
  middle_blob_.ReshapeLike(*bottom[0]);
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  Dtype* middle_diffs = middle_blob_.mutable_cpu_diff();
  if (cluster_regular_ == "rawsubsqr-overall-cluster") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += 2 * buffer_datum;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "rawsubsqr-nonself-cluster") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        Dtype buffer_diffs = *numidx_datpt / (*numidx_datpt - 1);
        *middle_datpt += 2 * buffer_datum * buffer_diffs * buffer_diffs;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "rawsubsqr-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += (*numidx_datpt * 2 - 2) * buffer_datum / *numidx_datpt;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "rawsubsqr-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *middle_datpt += *numidx_datpt * 2 * buffer_datum / (*numidx_datpt - 1);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "logsubsqr-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = 2 * buffer_datum / (buffer_datum * buffer_datum + 1);
        *middle_datpt += (*numidx_datpt - 1) * buffer_datum / *numidx_datpt;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "logsubsqr-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += 2 * buffer_datum / (buffer_datum * buffer_datum + 1);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "expsubsqr-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = 2 * buffer_datum * exp(-buffer_datum * buffer_datum);
        *middle_datpt += (*numidx_datpt - 1) * buffer_datum / *numidx_datpt;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "expsubsqr-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += 2 * buffer_datum * exp(-buffer_datum * buffer_datum);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "rawsubabs-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
        *middle_datpt += (*numidx_datpt - 1) * buffer_dsign / *numidx_datpt;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "rawsubabs-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
        *middle_datpt += buffer_dsign;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "logsubabs-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
        buffer_datum = buffer_dsign / (abs(buffer_datum) + 1);
        *middle_datpt += (*numidx_datpt - 1) * buffer_datum / *numidx_datpt;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "logsubabs-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += buffer_dsign / (abs(buffer_datum) + 1);
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "expsubabs-overall-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 1) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
        buffer_datum = buffer_dsign * exp(-abs(buffer_datum));
        *middle_datpt += (*numidx_datpt - 1) * buffer_datum / *numidx_datpt;
        *middle_difpt += 1;
      }
    }
  }
  else if (cluster_regular_ == "expsubabs-nonself-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_shift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
        if (static_cast<int>(*numidx_datpt) < 2) continue;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
        buffer_datum = *numidx_datpt * buffer_datum / (*numidx_datpt - 1);
        *middle_datpt += buffer_dsign * exp(-abs(buffer_datum));
        *middle_difpt += 1;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ScatterRegular_cpu(const vector<Blob<Dtype>*>& bottom) {
  middle_blob_.ReshapeLike(*bottom[0]);
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  Dtype* middle_diffs = middle_blob_.mutable_cpu_diff();
  if (scatter_regular_ == "rawsubsqr-samples-cluster") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *bottom_datpt;
          Dtype buffer_diffs = *avgsum_datpt - *avgsum_datit;
          *middle_datpt += 2 * (buffer_datum + buffer_diffs * *numidx_datpt / *numidx_datit);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "rawsubsqr-cluster-cluster") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *avgsum_datit;
          *middle_datpt += 2 * buffer_datum * (1 + *numidx_datpt) / *numidx_datit;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "rawsubsqr-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *bottom_datpt;
          *middle_datpt += 2 * buffer_datum;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "rawsubsqr-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *avgsum_datit;
          *middle_datpt += 2 * buffer_datum / *numidx_datit;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "logsubsqr-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *bottom_datpt;
          *middle_datpt += 2 * buffer_datum / (buffer_datum * buffer_datum + 1);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "logsubsqr-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *avgsum_datit;
          *middle_datpt += 2 * buffer_datum / (buffer_datum * buffer_datum + 1) / *numidx_datit;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "expsubsqr-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *bottom_datpt;
          *middle_datpt += 2 * buffer_datum * exp(-buffer_datum * buffer_datum);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "expsubsqr-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *avgsum_datit;
          *middle_datpt += 2 * buffer_datum * exp(-buffer_datum * buffer_datum) / *numidx_datit;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "rawsubabs-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *bottom_datpt;
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *middle_datpt += buffer_dsign;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "rawsubabs-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *avgsum_datit;
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *middle_datpt += buffer_dsign / *numidx_datit;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "logsubabs-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *bottom_datpt;
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *middle_datpt += buffer_dsign / (abs(buffer_datum) + 1);
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "logsubabs-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *avgsum_datit;
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *middle_datpt += buffer_dsign / (abs(buffer_datum) + 1) / *numidx_datit;
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "expsubabs-samples-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_datpt = bottom_datum + round_index; //bottom datum pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *bottom_datpt;
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *middle_datpt += buffer_dsign * exp(-abs(buffer_datum));
          *middle_difpt += 1;
        }
      }
    }
  }
  else if (scatter_regular_ == "expsubabs-average-average") {
    const int round_count = outer_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int outer_index = round_index / inner_numb_;
            Dtype* middle_datpt = middle_datum + round_index; //middle datum pointer
            Dtype* middle_difpt = middle_diffs + round_index; //middle diffs pointer
      const Dtype* bottom_labpt = bottom_label + outer_index * label_numb_; //bottom label pointer
                  *middle_datpt = *middle_difpt = 0;
      for (int label_index = 0; label_index < label_numb_; ++label_index, ++bottom_labpt) {
        if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
        if (static_cast<int>(*bottom_labpt) < 0) continue;
        const int numidx_drift = label_index * label_nmax_ + static_cast<int>(*bottom_labpt);
        const int avgsum_drift = inner_index + inner_numb_ * numidx_drift;
        const Dtype* numidx_datit = numidx_datum + numidx_drift; //num iterator
        const Dtype* avgsum_datit = avgsum_datum + avgsum_drift; //avg iterator
        if (static_cast<int>(*numidx_datit) < 1) continue;
        for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
          if (labmx_index == static_cast<int>(*bottom_labpt)) continue;
          const int numidx_shift = label_index * label_nmax_ + labmx_index;
          const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
          const Dtype* numidx_datpt = numidx_datum + numidx_shift; //num pointer
          const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift; //avg pointer
          if (static_cast<int>(*numidx_datpt) < 1) continue;
          Dtype buffer_datum = *avgsum_datpt - *avgsum_datit;
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *middle_datpt += buffer_dsign * exp(-abs(buffer_datum)) / *numidx_datit;
          *middle_difpt += 1;
        }
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::OdotterReshunt_cpu(const vector<Blob<Dtype>*>& bottom) {
  middle_blob_.ReshapeLike(*bottom[0]);
  const int* mapair_datum = mapair_blob_.cpu_data();
  const int* mapair_diffs = mapair_blob_.cpu_diff();
  const Dtype* maprop_datum = maprop_blob_.cpu_data();
  const Dtype* maprop_diffs = maprop_blob_.cpu_diff();
  const Dtype* storer_datum = storer_blob_.cpu_data();
  const Dtype* storer_diffs = storer_blob_.cpu_diff();
  Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  Dtype* middle_diffs = middle_blob_.mutable_cpu_diff();
  caffe_set(outer_numb_ * inner_numb_, Dtype(0), middle_datum);
  caffe_set(outer_numb_ * inner_numb_, Dtype(0), middle_diffs);
  for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
    for (int match_index = 0; match_index < match_numb_; ++match_index) {
      for (int label_index = 0; label_index < label_numb_; ++label_index) {
        const int mapair_shift = match_index * label_numb_ + label_index;
        const int storer_shift = inner_index + inner_numb_ * mapair_shift;
        const int* mapair_datpt = mapair_datum + mapair_shift;
        const int* mapair_difpt = mapair_diffs + mapair_shift;
        const int srcmid_shift = inner_index + inner_numb_ * *mapair_datpt;
        const int trgmid_shift = inner_index + inner_numb_ * *mapair_difpt;
        const Dtype* maprop_datpt = maprop_datum + mapair_shift;
        const Dtype* maprop_difpt = maprop_diffs + mapair_shift;
        const Dtype* storer_datpt = storer_datum + storer_shift;
        const Dtype* storer_difpt = storer_diffs + storer_shift;
        if (*mapair_datpt < 0 || *mapair_difpt < 0) continue;
        Dtype* srcmid_datpt = middle_datum + srcmid_shift;
        Dtype* srcmid_difpt = middle_diffs + srcmid_shift;
        Dtype* trgmid_datpt = middle_datum + trgmid_shift;
        Dtype* trgmid_difpt = middle_diffs + trgmid_shift;
        *srcmid_datpt += *storer_datpt * *maprop_datpt;
        *trgmid_datpt += *storer_difpt * *maprop_difpt;
        *srcmid_difpt += 1;
        *trgmid_difpt += 1;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::OdotterRegular_cpu(const vector<Blob<Dtype>*>& bottom) {
  vector<int> storer_shape(3);
  storer_shape[0] = match_numb_;
  storer_shape[1] = label_numb_;
  storer_shape[2] = inner_numb_;
  storer_blob_.Reshape(storer_shape);
  const int* mapair_datum = mapair_blob_.cpu_data();
  const int* mapair_diffs = mapair_blob_.cpu_diff();
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  const Dtype* medial_datum = medial_blob_.cpu_data();
  const Dtype* medial_diffs = medial_blob_.cpu_diff();
  Dtype* storer_datum = storer_blob_.mutable_cpu_data();
  Dtype* storer_diffs = storer_blob_.mutable_cpu_diff();
  if (odotter_regular_ == "rawsubsqr-overall-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_coeff = srclab_check / *numidx_datpt;
        const Dtype trgbuf_coeff = trglab_check / *numidx_datpt;
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        const Dtype srcbuf_diffs = 2 * srcbuf_datum * *medial_datpt;
        const Dtype trgbuf_diffs = 2 * trgbuf_datum * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srcbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
        *storer_difpt += trgbuf_diffs - trgbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
      }
    }
  }
  else if (odotter_regular_ == "rawsubsqr-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srclab_check * srcbuf_error;
        const Dtype avgsum_volum = *avgsum_datpt + trglab_check * trgbuf_error;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        const Dtype srcbuf_diffs = 2 * srcbuf_datum * *medial_datpt;
        const Dtype trgbuf_diffs = 2 * trgbuf_datum * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srclab_check * trgbuf_diffs / (*numidx_datpt - trglab_check);
        *storer_difpt += trgbuf_diffs - trglab_check * srcbuf_diffs / (*numidx_datpt - srclab_check);
      }
    }
  }
  else if (odotter_regular_ == "logsubsqr-overall-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_coeff = srclab_check / *numidx_datpt;
        const Dtype trgbuf_coeff = trglab_check / *numidx_datpt;
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        const Dtype srcbuf_diffs = 2 * srcbuf_datum / (srcbuf_datum * srcbuf_datum + 1) * *medial_datpt;
        const Dtype trgbuf_diffs = 2 * trgbuf_datum / (trgbuf_datum * trgbuf_datum + 1) * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srcbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
        *storer_difpt += trgbuf_diffs - trgbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
      }
    }
  }
  else if (odotter_regular_ == "logsubsqr-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srclab_check * srcbuf_error;
        const Dtype avgsum_volum = *avgsum_datpt + trglab_check * trgbuf_error;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        const Dtype srcbuf_diffs = 2 * srcbuf_datum / (srcbuf_datum * srcbuf_datum + 1) * *medial_datpt;
        const Dtype trgbuf_diffs = 2 * trgbuf_datum / (trgbuf_datum * trgbuf_datum + 1) * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srclab_check * trgbuf_diffs / (*numidx_datpt - trglab_check);
        *storer_difpt += trgbuf_diffs - trglab_check * srcbuf_diffs / (*numidx_datpt - srclab_check);
      }
    }
  }
  else if (odotter_regular_ == "expsubsqr-overall-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_coeff = srclab_check / *numidx_datpt;
        const Dtype trgbuf_coeff = trglab_check / *numidx_datpt;
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        const Dtype srcbuf_diffs = 2 * srcbuf_datum * exp(-srcbuf_datum * srcbuf_datum) * *medial_datpt;
        const Dtype trgbuf_diffs = 2 * trgbuf_datum * exp(-trgbuf_datum * trgbuf_datum) * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srcbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
        *storer_difpt += trgbuf_diffs - trgbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
      }
    }
  }
  else if (odotter_regular_ == "expsubsqr-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srclab_check * srcbuf_error;
        const Dtype avgsum_volum = *avgsum_datpt + trglab_check * trgbuf_error;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        const Dtype srcbuf_diffs = 2 * srcbuf_datum * exp(-srcbuf_datum * srcbuf_datum) * *medial_datpt;
        const Dtype trgbuf_diffs = 2 * trgbuf_datum * exp(-trgbuf_datum * trgbuf_datum) * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srclab_check * trgbuf_diffs / (*numidx_datpt - trglab_check);
        *storer_difpt += trgbuf_diffs - trglab_check * srcbuf_diffs / (*numidx_datpt - srclab_check);
      }
    }
  }
  else if (odotter_regular_ == "rawsubabs-overall-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_coeff = srclab_check / *numidx_datpt;
        const Dtype trgbuf_coeff = trglab_check / *numidx_datpt;
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        const Dtype srcbuf_dsign = srcbuf_datum < 0 ? -1 : (srcbuf_datum > 0 ? 1 : 0);
        const Dtype trgbuf_dsign = trgbuf_datum < 0 ? -1 : (trgbuf_datum > 0 ? 1 : 0);
        const Dtype srcbuf_diffs = srcbuf_dsign * *medial_datpt;
        const Dtype trgbuf_diffs = trgbuf_dsign * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srcbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
        *storer_difpt += trgbuf_diffs - trgbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
      }
    }
  }
  else if (odotter_regular_ == "rawsubabs-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srclab_check * srcbuf_error;
        const Dtype avgsum_volum = *avgsum_datpt + trglab_check * trgbuf_error;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        const Dtype srcbuf_dsign = srcbuf_datum < 0 ? -1 : (srcbuf_datum > 0 ? 1 : 0);
        const Dtype trgbuf_dsign = trgbuf_datum < 0 ? -1 : (trgbuf_datum > 0 ? 1 : 0);
        const Dtype srcbuf_diffs = srcbuf_dsign * *medial_datpt;
        const Dtype trgbuf_diffs = trgbuf_dsign * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srclab_check * trgbuf_diffs / (*numidx_datpt - trglab_check);
        *storer_difpt += trgbuf_diffs - trglab_check * srcbuf_diffs / (*numidx_datpt - srclab_check);
      }
    }
  }
  else if (odotter_regular_ == "logsubabs-overall-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_coeff = srclab_check / *numidx_datpt;
        const Dtype trgbuf_coeff = trglab_check / *numidx_datpt;
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        const Dtype srcbuf_dsign = srcbuf_datum < 0 ? -1 : (srcbuf_datum > 0 ? 1 : 0);
        const Dtype trgbuf_dsign = trgbuf_datum < 0 ? -1 : (trgbuf_datum > 0 ? 1 : 0);
        const Dtype srcbuf_diffs = srcbuf_dsign / (abs(srcbuf_datum) + 1) * *medial_datpt;
        const Dtype trgbuf_diffs = trgbuf_dsign / (abs(trgbuf_datum) + 1) * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srcbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
        *storer_difpt += trgbuf_diffs - trgbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
      }
    }
  }
  else if (odotter_regular_ == "logsubabs-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srclab_check * srcbuf_error;
        const Dtype avgsum_volum = *avgsum_datpt + trglab_check * trgbuf_error;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        const Dtype srcbuf_dsign = srcbuf_datum < 0 ? -1 : (srcbuf_datum > 0 ? 1 : 0);
        const Dtype trgbuf_dsign = trgbuf_datum < 0 ? -1 : (trgbuf_datum > 0 ? 1 : 0);
        const Dtype srcbuf_diffs = srcbuf_dsign / (abs(srcbuf_datum) + 1) * *medial_datpt;
        const Dtype trgbuf_diffs = trgbuf_dsign / (abs(trgbuf_datum) + 1) * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srclab_check * trgbuf_diffs / (*numidx_datpt - trglab_check);
        *storer_difpt += trgbuf_diffs - trglab_check * srcbuf_diffs / (*numidx_datpt - srclab_check);
      }
    }
  }
  else if (odotter_regular_ == "expsubabs-overall-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_coeff = srclab_check / *numidx_datpt;
        const Dtype trgbuf_coeff = trglab_check / *numidx_datpt;
        const Dtype srcbuf_datum = *srcbot_datpt - *avgsum_datpt;
        const Dtype trgbuf_datum = *trgbot_datpt - *avgsum_datpt;
        const Dtype srcbuf_dsign = srcbuf_datum < 0 ? -1 : (srcbuf_datum > 0 ? 1 : 0);
        const Dtype trgbuf_dsign = trgbuf_datum < 0 ? -1 : (trgbuf_datum > 0 ? 1 : 0);
        const Dtype srcbuf_diffs = srcbuf_dsign * exp(-abs(srcbuf_datum)) * *medial_datpt;
        const Dtype trgbuf_diffs = trgbuf_dsign * exp(-abs(trgbuf_datum)) * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srcbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
        *storer_difpt += trgbuf_diffs - trgbuf_coeff * (srcbuf_diffs + trgbuf_diffs);
      }
    }
  }
  else if (odotter_regular_ == "expsubabs-nonself-average") {
    const int round_count = match_numb_ * label_numb_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index = round_index % inner_numb_;
      const int label_index = round_index / inner_numb_ % label_numb_;
      const int match_index = round_index / inner_numb_ / label_numb_;
      const int mapair_shift = match_index * label_numb_ + label_index;
      const int* mapair_datpt = mapair_datum + mapair_shift;
      const int* mapair_difpt = mapair_diffs + mapair_shift;
      const int srcbot_shift = inner_index + inner_numb_ * *mapair_datpt;
      const int trgbot_shift = inner_index + inner_numb_ * *mapair_difpt;
      const int srclab_shift = label_index + label_numb_ * *mapair_datpt;
      const int trglab_shift = label_index + label_numb_ * *mapair_difpt;
      const int medial_shift = mapair_shift * label_nmax_;
      const Dtype* srcbot_datpt = bottom_datum + srcbot_shift;
      const Dtype* trgbot_datpt = bottom_datum + trgbot_shift;
      const Dtype* srcbot_labpt = bottom_label + srclab_shift;
      const Dtype* trgbot_labpt = bottom_label + trglab_shift;
      const Dtype* medial_datpt = medial_datum + medial_shift;
      const Dtype* medial_difpt = medial_diffs + medial_shift;
            Dtype* storer_datpt = storer_datum + round_index;
            Dtype* storer_difpt = storer_diffs + round_index;
                  *storer_datpt = *storer_difpt = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index, ++medial_datpt, ++medial_difpt) {
        if (*medial_datpt * *medial_difpt > 0) continue;
        const bool srclab_check = (static_cast<int>(*srcbot_labpt) == labmx_index);
        const bool trglab_check = (static_cast<int>(*trgbot_labpt) == labmx_index);
        const int numidx_shift = label_index * label_nmax_ + labmx_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* numidx_datpt = numidx_datum + numidx_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype srcbuf_error = (*avgsum_datpt - *srcbot_datpt) / (*numidx_datpt - srclab_check);
        const Dtype trgbuf_error = (*avgsum_datpt - *trgbot_datpt) / (*numidx_datpt - trglab_check);
        const Dtype avgsum_value = *avgsum_datpt + srclab_check * srcbuf_error;
        const Dtype avgsum_volum = *avgsum_datpt + trglab_check * trgbuf_error;
        const Dtype srcbuf_datum = *srcbot_datpt - avgsum_value;
        const Dtype trgbuf_datum = *trgbot_datpt - avgsum_volum;
        const Dtype srcbuf_dsign = srcbuf_datum < 0 ? -1 : (srcbuf_datum > 0 ? 1 : 0);
        const Dtype trgbuf_dsign = trgbuf_datum < 0 ? -1 : (trgbuf_datum > 0 ? 1 : 0);
        const Dtype srcbuf_diffs = srcbuf_dsign * exp(-abs(srcbuf_datum)) * *medial_datpt;
        const Dtype trgbuf_diffs = trgbuf_dsign * exp(-abs(trgbuf_datum)) * *medial_difpt;
        *storer_datpt += srcbuf_diffs - srclab_check * trgbuf_diffs / (*numidx_datpt - trglab_check);
        *storer_difpt += trgbuf_diffs - trglab_check * srcbuf_diffs / (*numidx_datpt - srclab_check);
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::OvalizeRegular_cpu(const vector<Blob<Dtype>*>& bottom) {
  Dtype* medial_datum = medial_blob_.mutable_cpu_data();
  Dtype* medial_diffs = medial_blob_.mutable_cpu_diff();
  if (ovalize_regular_ == "rawsubsqr-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += *medial_datpt - *medial_difpt;
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          *medial_datpt = 0 < buffer_diffs ? (+2 * buffer_datum / buffer_diffs) : 0;
          *medial_difpt = 0 < buffer_diffs ? (-2 * buffer_datum / buffer_diffs) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "rawsubsqr-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          *medial_datpt = buffer_diffs * sqrt(*medial_datpt);
          *medial_difpt = buffer_diffs * sqrt(*medial_difpt);
          *medial_datpt = 0 < *medial_datpt ? (+buffer_datum / *medial_datpt) : 0;
          *medial_difpt = 0 < *medial_difpt ? (-buffer_datum / *medial_difpt) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "logsubsqr-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += *medial_datpt - *medial_difpt;
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          *medial_datpt = *medial_difpt = (buffer_datum * buffer_datum + 1) * buffer_diffs;
          *medial_datpt = 0 < *medial_datpt ? (+2 * buffer_datum / *medial_datpt) : 0;
          *medial_difpt = 0 < *medial_difpt ? (-2 * buffer_datum / *medial_difpt) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "logsubsqr-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          *medial_datpt = (buffer_datum * buffer_datum + 1) * buffer_diffs * sqrt(*medial_datpt);
          *medial_difpt = (buffer_datum * buffer_datum + 1) * buffer_diffs * sqrt(*medial_difpt);
          *medial_datpt = 0 < *medial_datpt ? (+buffer_datum / *medial_datpt) : 0;
          *medial_difpt = 0 < *medial_difpt ? (-buffer_datum / *medial_difpt) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "expsubsqr-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += *medial_datpt - *medial_difpt;
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          *medial_datpt = 0 < buffer_diffs ? (+2 * buffer_datum * exp(-buffer_datum * buffer_datum) / buffer_diffs) : 0;
          *medial_difpt = 0 < buffer_diffs ? (-2 * buffer_datum * exp(-buffer_datum * buffer_datum) / buffer_diffs) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "expsubsqr-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          *medial_datpt = buffer_diffs * sqrt(*medial_datpt);
          *medial_difpt = buffer_diffs * sqrt(*medial_difpt);
          *medial_datpt = 0 < *medial_datpt ? (+buffer_datum * exp(-buffer_datum * buffer_datum) / *medial_datpt) : 0;
          *medial_difpt = 0 < *medial_difpt ? (-buffer_datum * exp(-buffer_datum * buffer_datum) / *medial_difpt) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "rawsubabs-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += *medial_datpt - *medial_difpt;
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *medial_datpt = 0 < buffer_diffs ? (+buffer_dsign / buffer_diffs) : 0;
          *medial_difpt = 0 < buffer_diffs ? (-buffer_dsign / buffer_diffs) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "rawsubabs-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *medial_datpt = buffer_diffs * sqrt(*medial_datpt);
          *medial_difpt = buffer_diffs * sqrt(*medial_difpt);
          *medial_datpt = 0 < *medial_datpt ? (+buffer_dsign / *medial_datpt) : 0;
          *medial_difpt = 0 < *medial_difpt ? (-buffer_dsign / *medial_difpt) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "logsubabs-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += *medial_datpt - *medial_difpt;
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *medial_datpt = *medial_difpt = (abs(buffer_datum) + 1) * buffer_diffs;
          *medial_datpt = 0 < *medial_datpt ? (+buffer_dsign / *medial_datpt) : 0;
          *medial_difpt = 0 < *medial_difpt ? (-buffer_dsign / *medial_difpt) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "logsubabs-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *medial_datpt = (abs(buffer_datum) + 1) * buffer_diffs * sqrt(*medial_datpt);
          *medial_difpt = (abs(buffer_datum) + 1) * buffer_diffs * sqrt(*medial_difpt);
          *medial_datpt = 0 < *medial_datpt ? (+buffer_dsign / *medial_datpt) : 0;
          *medial_difpt = 0 < *medial_difpt ? (-buffer_dsign / *medial_difpt) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "expsubabs-origins-origins") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += *medial_datpt - *medial_difpt;
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *medial_datpt = 0 < buffer_diffs ? (+buffer_dsign * exp(-abs(buffer_datum)) / buffer_diffs) : 0;
          *medial_difpt = 0 < buffer_diffs ? (-buffer_dsign * exp(-abs(buffer_datum)) / buffer_diffs) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
  else if (ovalize_regular_ == "expsubabs-sqroots-sqroots") {
    const int round_count = match_numb_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int medial_shift = round_index * label_nmax_ - 1;
      Dtype* medial_datpt = medial_datum + medial_shift;
      Dtype* medial_difpt = medial_diffs + medial_shift;
      Dtype buffer_datum = 0, buffer_diffs = 0;
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        ++medial_datpt; ++medial_difpt;
        if (*medial_datpt < 0 || *medial_difpt < 0) continue;
        buffer_datum += sqrt(*medial_datpt) - sqrt(*medial_difpt);
        buffer_diffs += 1;
      }
      for (int labmx_index = 0; labmx_index < label_nmax_; ++labmx_index) {
        if (*medial_datpt < 0 || *medial_difpt < 0) {
          *medial_datpt = *medial_difpt = -1;
        } else {
          Dtype buffer_dsign = buffer_datum < 0 ? -1 : (buffer_datum > 0 ? 1 : 0);
          *medial_datpt = buffer_diffs * sqrt(*medial_datpt);
          *medial_difpt = buffer_diffs * sqrt(*medial_difpt);
          *medial_datpt = 0 < *medial_datpt ? (+buffer_dsign * exp(-abs(buffer_datum)) / *medial_datpt) : 0;
          *medial_difpt = 0 < *medial_difpt ? (-buffer_dsign * exp(-abs(buffer_datum)) / *medial_difpt) : 0;
        }
        --medial_datpt; --medial_difpt;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::PredictTesting_cpu(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  Dtype* topper_datum = top[outputs_activate_[1]]->mutable_cpu_data();
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  if (predict_measure_ == "rawsubsqr-overall-average" || predict_measure_ == "rawsubsqr-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= buffer_datum * buffer_datum;
      }
    }
  }
  else if (predict_measure_ == "logsubsqr-overall-average" || predict_measure_ == "logsubsqr-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= log(buffer_datum * buffer_datum + 1);
      }
    }
  }
  else if (predict_measure_ == "expsubsqr-overall-average" || predict_measure_ == "expsubsqr-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= 1 - exp(-buffer_datum * buffer_datum);
      }
    }
  }
  else if (predict_measure_ == "rawsubabs-overall-average" || predict_measure_ == "rawsubabs-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= abs(buffer_datum);
      }
    }
  }
  else if (predict_measure_ == "logsubabs-overall-average" || predict_measure_ == "logsubabs-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= log(abs(buffer_datum) + 1);
      }
    }
  }
  else if (predict_measure_ == "expsubabs-overall-average" || predict_measure_ == "expsubabs-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= 1 - exp(-abs(buffer_datum));
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::PredictMeasure_cpu(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  Dtype* topper_datum = top[outputs_activate_[1]]->mutable_cpu_data();
  const Dtype* bottom_datum = bottom[0]->cpu_data();
  const Dtype* bottom_label = bottom[1]->cpu_data();
  const Dtype* numidx_datum = this->blobs_[0]->cpu_data();
  const Dtype* avgsum_datum = this->blobs_[1]->cpu_data();
  if (predict_measure_ == "rawsubsqr-overall-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= buffer_datum * buffer_datum;
      }
    }
  }
  else if (predict_measure_ == "rawsubsqr-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const bool botlab_check = (static_cast<int>(*bottom_labpt) == labmx_index);
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype  avgsum_error = (*avgsum_datpt - *bottom_datpt) / (*numidx_datpt - 1);
        const Dtype  avgsum_value = *avgsum_datpt + avgsum_error * botlab_check;
        Dtype buffer_datum = *bottom_datpt - avgsum_value;
        *topper_datpt -= buffer_datum * buffer_datum;
      }
    }
  }
  else if (predict_measure_ == "logsubsqr-overall-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= log(buffer_datum * buffer_datum + 1);
      }
    }
  }
  else if (predict_measure_ == "logsubsqr-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const bool botlab_check = (static_cast<int>(*bottom_labpt) == labmx_index);
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype  avgsum_error = (*avgsum_datpt - *bottom_datpt) / (*numidx_datpt - 1);
        const Dtype  avgsum_value = *avgsum_datpt + avgsum_error * botlab_check;
        Dtype buffer_datum = *bottom_datpt - avgsum_value;
        *topper_datpt -= log(buffer_datum * buffer_datum + 1);
      }
    }
  }
  else if (predict_measure_ == "expsubsqr-overall-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= 1 - exp(-buffer_datum * buffer_datum);
      }
    }
  }
  else if (predict_measure_ == "expsubsqr-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const bool botlab_check = (static_cast<int>(*bottom_labpt) == labmx_index);
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype  avgsum_error = (*avgsum_datpt - *bottom_datpt) / (*numidx_datpt - 1);
        const Dtype  avgsum_value = *avgsum_datpt + avgsum_error * botlab_check;
        Dtype buffer_datum = *bottom_datpt - avgsum_value;
        *topper_datpt -= 1 - exp(-buffer_datum * buffer_datum);
      }
    }
  }
  else if (predict_measure_ == "rawsubabs-overall-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= abs(buffer_datum);
      }
    }
  }
  else if (predict_measure_ == "rawsubabs-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const bool botlab_check = (static_cast<int>(*bottom_labpt) == labmx_index);
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype  avgsum_error = (*avgsum_datpt - *bottom_datpt) / (*numidx_datpt - 1);
        const Dtype  avgsum_value = *avgsum_datpt + avgsum_error * botlab_check;
        Dtype buffer_datum = *bottom_datpt - avgsum_value;
        *topper_datpt -= abs(buffer_datum);
      }
    }
  }
  else if (predict_measure_ == "logsubabs-overall-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= log(abs(buffer_datum) + 1);
      }
    }
  }
  else if (predict_measure_ == "logsubabs-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const bool botlab_check = (static_cast<int>(*bottom_labpt) == labmx_index);
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype  avgsum_error = (*avgsum_datpt - *bottom_datpt) / (*numidx_datpt - 1);
        const Dtype  avgsum_value = *avgsum_datpt + avgsum_error * botlab_check;
        Dtype buffer_datum = *bottom_datpt - avgsum_value;
        *topper_datpt -= log(abs(buffer_datum) + 1);
      }
    }
  }
  else if (predict_measure_ == "expsubabs-overall-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 1) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        Dtype buffer_datum = *bottom_datpt - *avgsum_datpt;
        *topper_datpt -= 1 - exp(-abs(buffer_datum));
      }
    }
  }
  else if (predict_measure_ == "expsubabs-nonself-average") {
    const int round_count = outer_numb_ * label_nmax_ * label_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int label_index = round_index % label_numb_;
      const int labmx_index = round_index / label_numb_ % label_nmax_;
      const int outer_index = round_index / label_numb_ / label_nmax_;
      const int botlab_shift = outer_index * label_numb_ + label_index;
      const int numidx_shift = label_index * label_nmax_ + labmx_index;
      const Dtype* bottom_labpt = bottom_label + botlab_shift;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
      const bool botlab_check = (static_cast<int>(*bottom_labpt) == labmx_index);
            Dtype* topper_datpt = topper_datum + round_index;
                  *topper_datpt = 0;
      if (static_cast<int>(*bottom_labpt) >= label_nmax_) continue;
      if (static_cast<int>(*bottom_labpt) < 0) continue;
      if (static_cast<int>(*numidx_datpt) < 2) continue;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        const int bottom_shift = outer_index * inner_numb_ + inner_index;
        const int avgsum_shift = inner_index + inner_numb_ * numidx_shift;
        const Dtype* bottom_datpt = bottom_datum + bottom_shift;
        const Dtype* avgsum_datpt = avgsum_datum + avgsum_shift;
        const Dtype  avgsum_error = (*avgsum_datpt - *bottom_datpt) / (*numidx_datpt - 1);
        const Dtype  avgsum_value = *avgsum_datpt + avgsum_error * botlab_check;
        Dtype buffer_datum = *bottom_datpt - avgsum_value;
        *topper_datpt -= 1 - exp(-abs(buffer_datum));
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ClusterBackward_cpu(const vector<Blob<Dtype>*>& top, const vector<Blob<Dtype>*>& bottom) {
  const Dtype* topper_diffs = outputs_activate_[0] != -1 ?
                          top[outputs_activate_[0]]->cpu_diff() : NULL;
        Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  const Dtype* middle_diffs = middle_blob_.cpu_diff();
  Dtype* bottom_diffs = bottom[0]->mutable_cpu_diff();
  if (cluster_clipmode_ == "sample-diff-based") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_ + inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      const Dtype* middle_difpt = middle_diffs + outer_index * inner_numb_;
      const Dtype* topper_difpt = topper_diffs + outer_index * inner_numb_;
            Dtype  sumsqr_datum = 0;
            Dtype  sumsqr_diffs = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        sumsqr_datum += 0 < *middle_difpt ? (*middle_datpt * *middle_datpt / *middle_difpt / *middle_difpt) : 0;
        sumsqr_diffs += *topper_difpt * *topper_difpt;
        ++middle_datpt; ++middle_difpt; ++topper_difpt;
      }
      const Dtype coeffi_alpha = cluster_clipactv_ || sumsqr_diffs < sumsqr_datum ?
        (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * cluster_clipprop_) : 0) : cluster_clipprop_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        --middle_datpt; --middle_difpt; --bottom_difpt;
        *bottom_difpt += 0 < *middle_difpt ? (coeffi_alpha * *middle_datpt / *middle_difpt) : 0;
      }
    }
  }
  else if (cluster_clipmode_ == "sample-norm-based") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_ + inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      const Dtype* middle_difpt = middle_diffs + outer_index * inner_numb_;
            Dtype  sumsqr_datum = 0;
      const Dtype  sumsqr_diffs = cluster_clipnorm_ * cluster_clipnorm_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        sumsqr_datum += 0 < *middle_difpt ? (*middle_datpt * *middle_datpt / *middle_difpt / *middle_difpt) : 0;
        ++middle_datpt; ++middle_difpt;
      }
      const Dtype coeffi_alpha = cluster_clipactv_ || sumsqr_diffs < sumsqr_datum ?
        (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * cluster_clipprop_) : 0) : cluster_clipprop_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        --middle_datpt; --middle_difpt; --bottom_difpt;
        *bottom_difpt += 0 < *middle_difpt ? (coeffi_alpha * *middle_datpt / *middle_difpt) : 0;
      }
    }
  }
  else if (cluster_clipmode_ == "blobal-diff-based") {
    caffe_div_nz(outer_numb_ * inner_numb_, middle_datum, middle_diffs, middle_datum);
    Dtype sumsqr_diffs = caffe_cpu_dot(outer_numb_ * inner_numb_, topper_diffs, topper_diffs);
    Dtype sumsqr_datum = caffe_cpu_dot(outer_numb_ * inner_numb_, middle_datum, middle_datum);
    const Dtype coeffi_alpha = cluster_clipactv_ || sumsqr_diffs < sumsqr_datum ?
      (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * cluster_clipprop_) : 0) : cluster_clipprop_;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += coeffi_alpha * *middle_datpt;
        ++middle_datpt; ++bottom_difpt;
      }
    }
  }
  else if (cluster_clipmode_ == "blobal-norm-based") {
    caffe_div_nz(outer_numb_ * inner_numb_, middle_datum, middle_diffs, middle_datum);
    Dtype sumsqr_diffs = cluster_clipnorm_ * cluster_clipnorm_;
    Dtype sumsqr_datum = caffe_cpu_dot(outer_numb_ * inner_numb_, middle_datum, middle_datum);
    const Dtype coeffi_alpha = cluster_clipactv_ || sumsqr_diffs < sumsqr_datum ?
      (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * cluster_clipprop_) : 0) : cluster_clipprop_;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += coeffi_alpha * *middle_datpt;
        ++middle_datpt; ++bottom_difpt;
      }
    }
  }
  else if (cluster_clipmode_ == "unclipped") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      const Dtype* middle_difpt = middle_diffs + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += 0 < *middle_difpt ? (cluster_clipprop_ * *middle_datpt / *middle_difpt) : 0;
        ++middle_datpt; ++middle_difpt; ++bottom_difpt;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::ScatterBackward_cpu(const vector<Blob<Dtype>*>& top, const vector<Blob<Dtype>*>& bottom) {
  const Dtype* topper_diffs = outputs_activate_[0] != -1 ?
                          top[outputs_activate_[0]]->cpu_diff() : NULL;
        Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  const Dtype* middle_diffs = middle_blob_.cpu_diff();
  Dtype* bottom_diffs = bottom[0]->mutable_cpu_diff();
  if (scatter_clipmode_ == "sample-diff-based") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_ + inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      const Dtype* middle_difpt = middle_diffs + outer_index * inner_numb_;
      const Dtype* topper_difpt = topper_diffs + outer_index * inner_numb_;
            Dtype  sumsqr_datum = 0;
            Dtype  sumsqr_diffs = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        sumsqr_datum += 0 < *middle_difpt ? (*middle_datpt * *middle_datpt / *middle_difpt / *middle_difpt) : 0;
        sumsqr_diffs += *topper_difpt * *topper_difpt;
        ++middle_datpt; ++middle_difpt; ++topper_difpt;
      }
      const Dtype coeffi_alpha = scatter_clipactv_ || sumsqr_diffs < sumsqr_datum ?
        (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * scatter_clipprop_) : 0) : scatter_clipprop_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        --middle_datpt; --middle_difpt; --bottom_difpt;
        *bottom_difpt += 0 < *middle_difpt ? (coeffi_alpha * *middle_datpt / *middle_difpt) : 0;
      }
    }
  }
  else if (scatter_clipmode_ == "sample-norm-based") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_ + inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      const Dtype* middle_difpt = middle_diffs + outer_index * inner_numb_;
            Dtype  sumsqr_datum = 0;
      const Dtype  sumsqr_diffs = scatter_clipnorm_ * scatter_clipnorm_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        sumsqr_datum += 0 < *middle_difpt ? (*middle_datpt * *middle_datpt / *middle_difpt / *middle_difpt) : 0;
        ++middle_datpt; ++middle_difpt;
      }
      const Dtype coeffi_alpha = scatter_clipactv_ || sumsqr_diffs < sumsqr_datum ?
        (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * scatter_clipprop_) : 0) : scatter_clipprop_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        --middle_datpt; --middle_difpt; --bottom_difpt;
        *bottom_difpt += 0 < *middle_difpt ? (coeffi_alpha * *middle_datpt / *middle_difpt) : 0;
      }
    }
  }
  else if (scatter_clipmode_ == "blobal-diff-based") {
    caffe_div_nz(outer_numb_ * inner_numb_, middle_datum, middle_diffs, middle_datum);
    Dtype sumsqr_diffs = caffe_cpu_dot(outer_numb_ * inner_numb_, topper_diffs, topper_diffs);
    Dtype sumsqr_datum = caffe_cpu_dot(outer_numb_ * inner_numb_, middle_datum, middle_datum);
    const Dtype coeffi_alpha = scatter_clipactv_ || sumsqr_diffs < sumsqr_datum ?
      (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * scatter_clipprop_) : 0) : scatter_clipprop_;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += coeffi_alpha * *middle_datpt;
        ++middle_datpt; ++bottom_difpt;
      }
    }
  }
  else if (scatter_clipmode_ == "blobal-norm-based") {
    caffe_div_nz(outer_numb_ * inner_numb_, middle_datum, middle_diffs, middle_datum);
    Dtype sumsqr_diffs = scatter_clipnorm_ * scatter_clipnorm_;
    Dtype sumsqr_datum = caffe_cpu_dot(outer_numb_ * inner_numb_, middle_datum, middle_datum);
    const Dtype coeffi_alpha = scatter_clipactv_ || sumsqr_diffs < sumsqr_datum ?
      (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * scatter_clipprop_) : 0) : scatter_clipprop_;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += coeffi_alpha * *middle_datpt;
        ++middle_datpt; ++bottom_difpt;
      }
    }
  }
  else if (scatter_clipmode_ == "unclipped") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      const Dtype* middle_difpt = middle_diffs + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += 0 < *middle_difpt ? (scatter_clipprop_ * *middle_datpt / *middle_difpt) : 0;
        ++middle_datpt; ++middle_difpt; ++bottom_difpt;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::TopdiffBackward_cpu(const vector<Blob<Dtype>*>& top, const vector<Blob<Dtype>*>& bottom) {
  const Dtype* topper_diffs = top[outputs_activate_[0]]->cpu_diff();
  Dtype* bottom_diffs = bottom[0]->mutable_cpu_diff();
  if (topdiff_clipmode_ == "sample-norm-based") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_ + inner_numb_;
      const Dtype* topper_difpt = topper_diffs + outer_index * inner_numb_;
            Dtype  sumsqr_datum = 0;
      const Dtype  sumsqr_diffs = topdiff_clipnorm_ * topdiff_clipnorm_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        sumsqr_datum += *topper_difpt * *topper_difpt;
        ++topper_difpt;
      }
      const Dtype coeffi_alpha = topdiff_clipactv_ || sumsqr_diffs < sumsqr_datum ?
        (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * topdiff_clipprop_) : 0) : topdiff_clipprop_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        --topper_difpt; --bottom_difpt;
        *bottom_difpt += coeffi_alpha * *topper_difpt;
      }
    }
  }
  else if (topdiff_clipmode_ == "blobal-norm-based") {
    Dtype sumsqr_diffs = topdiff_clipnorm_ * topdiff_clipnorm_;
    Dtype sumsqr_datum = caffe_cpu_dot(outer_numb_ * inner_numb_, topper_diffs, topper_diffs);
    const Dtype coeffi_alpha = topdiff_clipactv_ || sumsqr_diffs < sumsqr_datum ?
      (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * topdiff_clipprop_) : 0) : topdiff_clipprop_;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* topper_difpt = topper_diffs + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += coeffi_alpha * *topper_difpt;
        ++topper_difpt; ++bottom_difpt;
      }
    }
  }
  else if (topdiff_clipmode_ == "unclipped") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* topper_difpt = topper_diffs + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += topdiff_clipprop_ * *topper_difpt;
        ++topper_difpt; ++bottom_difpt;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::OvalizeBackward_cpu(const vector<Blob<Dtype>*>& top, const vector<Blob<Dtype>*>& bottom) {
  const Dtype* topper_diffs = outputs_activate_[0] != -1 ?
                          top[outputs_activate_[0]]->cpu_diff() : NULL;
        Dtype* middle_datum = middle_blob_.mutable_cpu_data();
  const Dtype* middle_diffs = middle_blob_.cpu_diff();
  Dtype* bottom_diffs = bottom[0]->mutable_cpu_diff();
  if (ovalize_clipmode_ == "sample-diff-based") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_ + inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      const Dtype* middle_difpt = middle_diffs + outer_index * inner_numb_;
      const Dtype* topper_difpt = topper_diffs + outer_index * inner_numb_;
            Dtype  sumsqr_datum = 0;
            Dtype  sumsqr_diffs = 0;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        sumsqr_datum += 0 < *middle_difpt ? (*middle_datpt * *middle_datpt / *middle_difpt / *middle_difpt) : 0;
        sumsqr_diffs += *topper_difpt * *topper_difpt;
        ++middle_datpt; ++middle_difpt; ++topper_difpt;
      }
      const Dtype coeffi_alpha = ovalize_clipactv_ || sumsqr_diffs < sumsqr_datum ?
        (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * ovalize_clipprop_) : 0) : ovalize_clipprop_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        --middle_datpt; --middle_difpt; --bottom_difpt;
        *bottom_difpt += 0 < *middle_difpt ? (coeffi_alpha * *middle_datpt / *middle_difpt) : 0;
      }
    }
  }
  else if (ovalize_clipmode_ == "sample-norm-based") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_ + inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      const Dtype* middle_difpt = middle_diffs + outer_index * inner_numb_;
            Dtype  sumsqr_datum = 0;
      const Dtype  sumsqr_diffs = ovalize_clipnorm_ * ovalize_clipnorm_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        sumsqr_datum += 0 < *middle_difpt ? (*middle_datpt * *middle_datpt / *middle_difpt / *middle_difpt) : 0;
        ++middle_datpt; ++middle_difpt;
      }
      const Dtype coeffi_alpha = ovalize_clipactv_ || sumsqr_diffs < sumsqr_datum ?
        (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * ovalize_clipprop_) : 0) : ovalize_clipprop_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        --middle_datpt; --middle_difpt; --bottom_difpt;
        *bottom_difpt += 0 < *middle_difpt ? (coeffi_alpha * *middle_datpt / *middle_difpt) : 0;
      }
    }
  }
  else if (ovalize_clipmode_ == "blobal-diff-based") {
    caffe_div_nz(outer_numb_ * inner_numb_, middle_datum, middle_diffs, middle_datum);
    Dtype sumsqr_diffs = caffe_cpu_dot(outer_numb_ * inner_numb_, topper_diffs, topper_diffs);
    Dtype sumsqr_datum = caffe_cpu_dot(outer_numb_ * inner_numb_, middle_datum, middle_datum);
    const Dtype coeffi_alpha = ovalize_clipactv_ || sumsqr_diffs < sumsqr_datum ?
      (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * ovalize_clipprop_) : 0) : ovalize_clipprop_;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += coeffi_alpha * *middle_datpt;
        ++middle_datpt; ++bottom_difpt;
      }
    }
  }
  else if (ovalize_clipmode_ == "blobal-norm-based") {
    caffe_div_nz(outer_numb_ * inner_numb_, middle_datum, middle_diffs, middle_datum);
    Dtype sumsqr_diffs = ovalize_clipnorm_ * ovalize_clipnorm_;
    Dtype sumsqr_datum = caffe_cpu_dot(outer_numb_ * inner_numb_, middle_datum, middle_datum);
    const Dtype coeffi_alpha = ovalize_clipactv_ || sumsqr_diffs < sumsqr_datum ?
      (0 < sumsqr_datum ? (sqrt(sumsqr_diffs / sumsqr_datum) * ovalize_clipprop_) : 0) : ovalize_clipprop_;
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += coeffi_alpha * *middle_datpt;
        ++middle_datpt; ++bottom_difpt;
      }
    }
  }
  else if (ovalize_clipmode_ == "unclipped") {
    for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
            Dtype* bottom_difpt = bottom_diffs + outer_index * inner_numb_;
      const Dtype* middle_datpt = middle_datum + outer_index * inner_numb_;
      const Dtype* middle_difpt = middle_diffs + outer_index * inner_numb_;
      for (int inner_index = 0; inner_index < inner_numb_; ++inner_index) {
        *bottom_difpt += 0 < *middle_difpt ? (ovalize_clipprop_ * *middle_datpt / *middle_difpt) : 0;
        ++middle_datpt; ++middle_difpt; ++bottom_difpt;
      }
    }
  }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::Forward_cpu(
    const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top,
    const bool preforward_flag) {
  if (this->phase_ != TRAIN) {
    if (outputs_activate_[1] != -1) {
      PredictTesting_cpu(bottom, top);
    }
    if (outputs_activate_[2] != -1) {
      clustr_blob_.Reshape(vector<int>(1, inner_numb_));
      caffe_set(clustr_blob_.count(), Dtype(0), clustr_blob_.mutable_cpu_data());
      caffe_set(clustr_blob_.count(), Dtype(0), clustr_blob_.mutable_cpu_diff());
      ClusterTesting_cpu(bottom); ClusterForward_cpu(top);
    }
    if (outputs_activate_[3] != -1) {
      scattr_blob_.Reshape(vector<int>(1, inner_numb_));
      caffe_set(scattr_blob_.count(), Dtype(0), scattr_blob_.mutable_cpu_data());
      caffe_set(scattr_blob_.count(), Dtype(0), scattr_blob_.mutable_cpu_diff());
      ScatterMeasure_cpu(bottom); ScatterForward_cpu(top);
    }
    if (outputs_activate_[4] != -1) {
      clusup_blob_.ReshapeLike(*this->blobs_[1]);
      caffe_set(clusup_blob_.count(), Dtype(0), clusup_blob_.mutable_cpu_data());
      caffe_set(clusup_blob_.count(), Dtype(0), clusup_blob_.mutable_cpu_diff());
      ClusupdTesting_cpu(bottom); ClusupdForward_cpu(top);
    }
    if (outputs_activate_[5] != -1) {
      scatup_blob_.ReshapeLike(*this->blobs_[1]);
      caffe_set(scatup_blob_.count(), Dtype(0), scatup_blob_.mutable_cpu_data());
      caffe_set(scatup_blob_.count(), Dtype(0), scatup_blob_.mutable_cpu_diff());
      ScatupdMeasure_cpu(bottom); ScatupdForward_cpu(top);
    }
    if (outputs_activate_[6] != -1) {
      ovaliz_blob_.Reshape(vector<int>(1, label_numb_));
      caffe_set(ovaliz_blob_.count(), Dtype(0), ovaliz_blob_.mutable_cpu_data());
      caffe_set(ovaliz_blob_.count(), Dtype(0), ovaliz_blob_.mutable_cpu_diff());
      OvalizeMatcher_cpu(bottom); OdotterTesting_cpu(bottom);
      OvalizeMeasure_cpu(bottom); OvalizeForward_cpu(top);
    }
    return;
  }
  if (!preforward_flag && preforward_tag_) {
    preforward_tag_ = false;
  } else if (preforward_flag && !preforward_tag_) {
    preforward_beg_ = preforward_tag_ = true;
  }
  if ((!average_initmode_ && !preforward_tag_) || (average_initmode_ && preforward_beg_)) {
    const int numidx_count = label_numb_ * label_nmax_;
    const int avgsum_count = inner_numb_ * numidx_count;
    caffe_set(numidx_count, Dtype(0), this->blobs_[0]->mutable_cpu_data());
    caffe_set(avgsum_count, Dtype(0), this->blobs_[1]->mutable_cpu_data());
    if (outputs_activate_[2] != -1) {
      clustr_blob_.Reshape(vector<int>(1, inner_numb_));
      caffe_set(clustr_blob_.count(), Dtype(0), clustr_blob_.mutable_cpu_data());
      caffe_set(clustr_blob_.count(), Dtype(0), clustr_blob_.mutable_cpu_diff());
    }
    if (outputs_activate_[3] != -1) {
      scattr_blob_.Reshape(vector<int>(1, inner_numb_));
      caffe_set(scattr_blob_.count(), Dtype(0), scattr_blob_.mutable_cpu_data());
      caffe_set(scattr_blob_.count(), Dtype(0), scattr_blob_.mutable_cpu_diff());
    }
    if (outputs_activate_[4] != -1) {
      clusup_blob_.ReshapeLike(*this->blobs_[1]);
      caffe_set(clusup_blob_.count(), Dtype(0), clusup_blob_.mutable_cpu_data());
      caffe_set(clusup_blob_.count(), Dtype(0), clusup_blob_.mutable_cpu_diff());
    }
    if (outputs_activate_[5] != -1) {
      scatup_blob_.ReshapeLike(*this->blobs_[1]);
      caffe_set(scatup_blob_.count(), Dtype(0), scatup_blob_.mutable_cpu_data());
      caffe_set(scatup_blob_.count(), Dtype(0), scatup_blob_.mutable_cpu_diff());
    }
    if (outputs_activate_[6] != -1) {
      ovaliz_blob_.Reshape(vector<int>(1, label_numb_));
      caffe_set(ovaliz_blob_.count(), Dtype(0), ovaliz_blob_.mutable_cpu_data());
      caffe_set(ovaliz_blob_.count(), Dtype(0), ovaliz_blob_.mutable_cpu_diff());
    }
  }
  if (preforward_beg_) preforward_beg_ = false;
  if ((!average_initmode_ && !preforward_tag_) || (average_initmode_ && preforward_tag_)) {
    medium_blob_.ReshapeLike(*this->blobs_[1]);
    const Dtype* bottom_datum = bottom[0]->cpu_data();
    const Dtype* bottom_label = bottom[1]->cpu_data();
    Dtype* numidx_datum = this->blobs_[0]->mutable_cpu_data();
    Dtype* avgsum_datum = this->blobs_[1]->mutable_cpu_data();
    Dtype* medium_datum = medium_blob_.mutable_cpu_data();
    Dtype* medium_diffs = medium_blob_.mutable_cpu_diff();
    const int round_count = label_numb_ * label_nmax_ * inner_numb_;
    for (int round_index = 0; round_index < round_count; ++round_index) {
      const int inner_index  = round_index % inner_numb_;
      const int labmx_index  = round_index / inner_numb_ % label_nmax_;
      const int label_index  = round_index / inner_numb_ / label_nmax_;
      const int numidx_shift = round_index / inner_numb_;
            Dtype* medium_datpt = medium_datum + round_index;
            Dtype* medium_difpt = medium_diffs + round_index;
      const Dtype* numidx_datpt = numidx_datum + numidx_shift;
            Dtype* avgsum_datpt = avgsum_datum + round_index;
                  *medium_datpt = *medium_difpt = 0;
      if (labmx_index != ignore_label_ && labmx_index != target_label_) {
        for (int outer_index = 0; outer_index < outer_numb_; ++outer_index) {
          const int bottom_shift = outer_index * inner_numb_ + inner_index;
          const int botlab_shift = outer_index * label_numb_ + label_index;
          const Dtype* bottom_datpt = bottom_datum + bottom_shift;
          const Dtype* bottom_labpt = bottom_label + botlab_shift;
          if (static_cast<int>(*bottom_labpt) != labmx_index) continue;
          *medium_datpt += *bottom_datpt;
          *medium_difpt += 1;
        }
        if (static_cast<int>(*medium_difpt) > 0) {
          *avgsum_datpt *= *numidx_datpt / (*numidx_datpt + *medium_difpt);
          *avgsum_datpt += *medium_datpt / (*numidx_datpt + *medium_difpt);
        }
      }
    }
    const int loops_count = label_numb_ * label_nmax_;
    for (int loops_index = 0; loops_index < loops_count; ++loops_index) {
            Dtype* numidx_datpt = numidx_datum + loops_index;
      const Dtype* medium_difpt = medium_diffs + inner_numb_ * loops_index;
      *numidx_datpt += *medium_difpt;
    }
  }
  if (outputs_activate_[1] != -1) { PredictMeasure_cpu(bottom, top); }
  if (outputs_activate_[2] != -1 && !preforward_tag_) { ClusterMeasure_cpu(bottom); ClusterForward_cpu(top); }
  if (outputs_activate_[3] != -1 && !preforward_tag_) { ScatterMeasure_cpu(bottom); ScatterForward_cpu(top); }
  if (outputs_activate_[4] != -1 && !preforward_tag_) { ClusupdMeasure_cpu(bottom); ClusupdForward_cpu(top); }
  if (outputs_activate_[5] != -1 && !preforward_tag_) { ScatupdMeasure_cpu(bottom); ScatupdForward_cpu(top); }
  if (outputs_activate_[6] != -1 && !preforward_tag_) { OvalizeMatcher_cpu(bottom); OdotterMeasure_cpu(bottom);
                                                        OvalizeMeasure_cpu(bottom); OvalizeForward_cpu(top); }
}

template <typename Dtype>
void HomoMeanLossLayer<Dtype>::Backward_cpu(
    const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom,
    const bool prebackward_flag) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type() << "Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    caffe_set(outer_numb_ * inner_numb_, Dtype(0), bottom[0]->mutable_cpu_diff());
    if (cluster_clipprop_ != Dtype(0) && cluster_interval_ &&
        solver_iter_ % cluster_interval_ >= cluster_postpone_ &&
        solver_iter_ % cluster_interval_ <  cluster_postpone_ + cluster_duration_) {
      ClusterRegular_cpu(bottom); ClusterBackward_cpu(top, bottom);
    }
    if (scatter_clipprop_ != Dtype(0) && scatter_interval_ &&
        solver_iter_ % scatter_interval_ >= scatter_postpone_ &&
        solver_iter_ % scatter_interval_ <  scatter_postpone_ + scatter_duration_) {
      ScatterRegular_cpu(bottom); ScatterBackward_cpu(top, bottom);
    }
    if (ovalize_clipprop_ != Dtype(0) && ovalize_interval_ &&
        solver_iter_ % ovalize_interval_ >= ovalize_postpone_ &&
        solver_iter_ % ovalize_interval_ <  ovalize_postpone_ + ovalize_duration_) {
      string  measure  = odotter_measure_;
      odotter_measure_ = odotter_regular_;
      OvalizeMatcher_cpu(bottom); OdotterMeasure_cpu(bottom);
      OvalizeRegular_cpu(bottom); OdotterRegular_cpu(bottom);
      OdotterReshunt_cpu(bottom); OvalizeBackward_cpu(top, bottom);
      odotter_measure_ = measure;
    }
    if (topdiff_clipprop_ != Dtype(0) && topdiff_interval_ &&
        solver_iter_ % topdiff_interval_ >= topdiff_postpone_ &&
        solver_iter_ % topdiff_interval_ <  topdiff_postpone_ + topdiff_duration_) {
      TopdiffBackward_cpu(top, bottom);
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(HomoMeanLossLayer);
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ClusterForward_gpu(const vector<Blob<Dtype>*>& top) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ScatterForward_gpu(const vector<Blob<Dtype>*>& top) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ClusupdForward_gpu(const vector<Blob<Dtype>*>& top) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ScatupdForward_gpu(const vector<Blob<Dtype>*>& top) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::OvalizeForward_gpu(const vector<Blob<Dtype>*>& top) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::OvalizeMatcher_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ClusterTesting_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ClusterMeasure_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ScatterMeasure_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ClusupdTesting_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ClusupdMeasure_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ScatupdMeasure_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::OdotterTesting_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::OdotterMeasure_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::OvalizeMeasure_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ClusterRegular_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ScatterRegular_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::OdotterReshunt_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::OdotterRegular_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::OvalizeRegular_gpu(const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::PredictTesting_gpu(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::PredictMeasure_gpu(const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ClusterBackward_gpu(const vector<Blob<Dtype>*>& top, const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::ScatterBackward_gpu(const vector<Blob<Dtype>*>& top, const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::TopdiffBackward_gpu(const vector<Blob<Dtype>*>& top, const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
template <typename Dtype> void HomoMeanLossLayer<Dtype>::OvalizeBackward_gpu(const vector<Blob<Dtype>*>& top, const vector<Blob<Dtype>*>& bottom) { NO_GPU; }
#endif

INSTANTIATE_CLASS(HomoMeanLossLayer);
REGISTER_LAYER_CLASS(HomoMeanLoss);
} // namespace caffe