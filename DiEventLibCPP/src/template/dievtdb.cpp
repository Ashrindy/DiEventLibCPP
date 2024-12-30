#include "../../include/template/dievtdb.h"
#include "../../include/template/dievtdbbin.h"

void dv::db::DiEventDataBase::read(const char* data, size_t size) {
	if (std::strncmp(data, "DiEvtDB", 7) == 0) {
		DiEventDataBaseBinary* dievtdb = new DiEventDataBaseBinary();
		dievtdb->read(data, size);
		version = dievtdb->version;
		nodes = dievtdb->nodes;
		elements = dievtdb->elements;
	}
}