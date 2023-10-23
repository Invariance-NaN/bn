#ifndef BN_QF20WQ_DATAFRAME_HPP_
#define BN_QF20WQ_DATAFRAME_HPP_

#include <string>
#include <vector>

class DataFrame {
  struct ColumnMeta {
    std::string name;
    unsigned cardinality;
  };

  std::vector<ColumnMeta> columns;
  std::vector<std::vector<unsigned>> data;

 public:
  bool conditional_independency_test(
    std::string x,
    std::string y,
    std::vector<std::string> zs
  ) {
    //
  }
};


#endif
