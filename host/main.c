#include <openenclave/host.h>
#include <stdio.h>
#include <stdlib.h>
#include "inference_u.h"

void ocall_print(const char* message) {
    printf("[Enclave] %s\n", message);
}

int main(int argc, char* argv[]) {
    oe_result_t result;
    oe_enclave_t* enclave = NULL;
    uint32_t flags = OE_ENCLAVE_FLAG_DEBUG;
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <enclave_path>\n", argv[0]);
        return 1;
    }
    
    // Create enclave
    result = oe_create_inference_enclave(
        argv[1],
        OE_ENCLAVE_TYPE_SGX,
        flags,
        NULL,
        0,
        &enclave
    );
    
    if (result != OE_OK) {
        fprintf(stderr, "Failed to create enclave: %s\n", 
                oe_result_str(result));
        return 1;
    }
    
    printf("Enclave created successfully\n");
    
    // Example: Load model
    float model_weights[10] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    result = ecall_load_model(
        enclave,
        (uint8_t*)model_weights,
        sizeof(model_weights)
    );
    
    if (result != OE_OK) {
        fprintf(stderr, "Failed to load model\n");
        goto cleanup;
    }
    
    // Example: Run inference
    float input[10] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    float output[1];
    
    result = ecall_inference(enclave, input, 10, output, 1);
    
    if (result == OE_OK) {
        printf("Inference result: %f\n", output[0]);
    }
    
    // Example: Generate attestation
    uint8_t nonce[16] = {0};
    uint8_t report[8192];
    size_t report_size;
    
    result = ecall_generate_attestation(
        enclave,
        nonce,
        sizeof(nonce),
        report,
        sizeof(report),
        &report_size
    );
    
    if (result == OE_OK) {
        printf("Attestation generated (%zu bytes)\n", report_size);
    }
    
cleanup:
    oe_terminate_enclave(enclave);
    printf("Enclave terminated\n");
    
    return 0;
}
