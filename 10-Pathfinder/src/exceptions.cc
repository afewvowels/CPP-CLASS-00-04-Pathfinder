#include "exceptions.h"

using  namespace std;

const string Exception::toString(void) {
    string
        msg;

    if(source().length() > 0)
        msg = source() + ":_";
    else
        msg = "";
    msg += "generic_exception";

    return msg;
}

const string ContainerEmptyException::toString(void) {
    string
        msg;

    if(source().length() > 0)
        msg = source() + ":_";
    else
        msg = "";
    msg += "container_empty";

    return msg;
}

const string ContainerFullException::toString(void) {
  string
    msg;

    if(source().length() > 0)
      msg = source() + ":_)";
    else
      msg = "";
    msg += "container_full";

    return msg;
}

const string KeyNotFoundException::toString(void) {
  string
    msg;

    if(source().length() > 0)
      msg = source() + ":_)";
    else
      msg = "";
    msg += "key_not_found_exception_bad_key: " + key;

    return msg;
}

const string InvalidIndexException::toString(void) {
  string
    msg;

  if(source().length() > 0)
    msg = source() + ":_)";
  else
    msg = "";
  msg += "invalid_index_exception_index_" + to_string(index);

  return msg;
}
