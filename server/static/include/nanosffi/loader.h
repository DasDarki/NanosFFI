//
// Created by DasDarki on 29.01.2025.
//

#ifndef LOADER_H
#define LOADER_H

class NanosFFI_Loader {
public:
  /**
  * Injects the NanosFFI into the nanos world server.
  */
  static void inject();

private:
  /**
   * Starts the binding process of the nanos world API.
   */
  static void bindAll();
};

#endif //LOADER_H
