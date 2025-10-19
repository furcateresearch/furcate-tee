# Furcate TEE

Trusted Execution Environment implementation for Furcate using Open Enclave SDK.

## Overview

Furcate TEE enables secure, verifiable machine learning inference and training on edge devices using hardware-backed trusted execution environments (Intel SGX, ARM TrustZone, AWS Nitro).

## Features

- **Secure Inference**: ML models execute in isolated enclaves
- **Cryptographic Attestation**: Proof of correct execution
- **Data Protection**: Sensitive data never leaves secure memory
- **Multi-Platform**: Intel SGX, ARM TrustZone, AWS Nitro support

## Prerequisites

- **Hardware**: Intel SGX-enabled CPU or compatible TEE
- **Software**: Open Enclave SDK 0.19+
- **OS**: Ubuntu 20.04+ or Windows 10+

## Installation

### Ubuntu

```bash
# Install Open Enclave
echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu focal main' | sudo tee /etc/apt/sources.list.d/intel-sgx.list
wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | sudo apt-key add -

sudo apt update
sudo apt install -y open-enclave

# Build Furcate TEE
mkdir build && cd build
cmake ..
make
```

## Quick Start

### Running Secure Inference

```bash
# Run in simulation mode (no SGX hardware required)
./build/host/furcate_tee_host simulate

# Run on SGX hardware
./build/host/furcate_tee_host hardware
```

### Example: Secure Model Inference

```c
// Load model into enclave
load_model_into_enclave(enclave, "model.onnx");

// Run inference on sensitive data
float input[10] = {1.0, 2.0, ...};
float output[1];
secure_inference(enclave, input, output);

// Generate attestation
attestation_report_t report;
generate_attestation(enclave, &report);
```

## Architecture

```
┌─────────────────────────────────────┐
│         Host Application            │
│   (Untrusted, manages enclave)      │
└──────────────┬──────────────────────┘
               │ ECALL/OCALL
┌──────────────▼──────────────────────┐
│       Enclave (TEE)                 │
│  ┌──────────────────────────────┐   │
│  │  ML Inference Engine         │   │
│  │  - Model weights (encrypted) │   │
│  │  - Secure computation        │   │
│  └──────────────────────────────┘   │
│  ┌──────────────────────────────┐   │
│  │  Attestation Module          │   │
│  │  - Quote generation          │   │
│  │  - Signature creation        │   │
│  └──────────────────────────────┘   │
└─────────────────────────────────────┘
```

## API Reference

### Enclave Functions (ECALLs)

```c
// Initialize ML model in enclave
oe_result_t load_model(const uint8_t* model_data, size_t size);

// Run inference
oe_result_t inference(const float* input, size_t input_size, 
                      float* output, size_t output_size);

// Generate attestation
oe_result_t generate_attestation(uint8_t* report, size_t* report_size);
```

## Security Considerations

- Models are encrypted before loading into enclave
- All computation occurs in isolated memory
- Attestation proves code integrity
- Side-channel attack mitigations enabled

## Performance

| Operation | Latency | Throughput |
|-----------|---------|------------|
| Inference (small model) | <10ms | 100 req/s |
| Attestation generation | <50ms | 20 req/s |
| Model loading | <100ms | - |

## Examples

See `examples/` directory for:
- `simple_inference/`: Basic inference example
- `federated_aggregation/`: Secure gradient aggregation
- `model_encryption/`: Encrypting models for enclave loading

## Development

### Building from Source

```bash
git clone https://github.com/furcateresearch/furcate-tee
cd furcate-tee
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### Running Tests

```bash
cd build
ctest --output-on-failure
```

## Docker Support

```bash
docker build -t furcate-tee -f docker/Dockerfile .
docker run -it --device=/dev/sgx furcate-tee
```

## License

MIT License - see LICENSE file

## Resources

- Open Enclave SDK: https://openenclave.io
- Documentation: https://docs.furcate.io/tee/
- Security: security@furcate.io
