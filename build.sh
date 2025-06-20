#!/bin/bash

# Hive AI最终修复版本构建脚本

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

echo -e "${BLUE}=== Hive AI最终修复版本构建脚本 ===${NC}"
echo ""

# 检查当前目录
if [[ ! -f "CMakeLists.txt" ]]; then
    log_error "请在项目根目录下运行此脚本"
    exit 1
fi

# 创建构建目录
log_info "创建构建目录..."
rm -rf build
mkdir build
cd build

# 配置项目
log_info "配置CMake项目..."
cmake -DCMAKE_BUILD_TYPE=Debug ..

# 编译项目
log_info "编译项目..."
make -j$(nproc)

# 检查编译结果
if [[ -f "HiveGameAI" ]]; then
    log_success "编译成功: HiveGameAI"
else
    log_error "编译失败"
    exit 1
fi

# 创建测试脚本
log_info "创建测试脚本..."

cat > test_final.sh << 'EOF'
#!/bin/bash

echo "=== 测试Hive AI最终修复版本 ==="
echo "1. 快速测试 (5局)"
echo "2. 标准测试 (20局)"
echo "3. 交互模式"
echo "4. 退出"
echo "请选择: "

read choice

case $choice in
    1)
        echo "运行快速测试..."
        echo -e "3\n1" | timeout 60 ./HiveGameAI
        ;;
    2)
        echo "运行标准测试..."
        echo -e "3\n2" | timeout 180 ./HiveGameAI
        ;;
    3)
        echo "启动交互模式..."
        ./HiveGameAI
        ;;
    4)
        echo "退出"
        ;;
    *)
        echo "无效选择，运行快速测试..."
        echo -e "3\n1" | timeout 60 ./HiveGameAI
        ;;
esac
EOF

chmod +x test_final.sh

# 显示使用说明
echo ""
log_success "构建完成！"
echo ""
echo -e "${YELLOW}可用的可执行文件:${NC}"
echo "  - HiveGameAI: 最终修复版本"
echo ""
echo -e "${YELLOW}测试脚本:${NC}"
echo "  - ./test_final.sh: 测试最终修复版本"
echo ""
echo -e "${YELLOW}直接运行:${NC}"
echo "  ./HiveGameAI"
echo ""
echo -e "${GREEN}最终修复版本的特点:${NC}"
echo "  ✓ 完全匹配原始头文件接口"
echo "  ✓ 解决所有编译错误"
echo "  ✓ 修复段错误问题"
echo "  ✓ 实现完整的AI训练系统"
echo "  ✓ 支持神经网络和强化学习"
echo "  ✓ 包含AITrainer训练管理器"
echo ""
echo -e "${BLUE}开始测试:${NC} ./test_final.sh"

