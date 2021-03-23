import torch 
import time 

class ResModule(torch.nn.Module):
    def __init__(self):
        super(ResModule, self).__init__()
        self.conv1 = torch.nn.Conv2d(128, 128, 3, 1, 1)
        self.bn1 = torch.nn.BatchNorm2d(128)
        self.relu1 = torch.nn.ReLU()
        self.conv2 = torch.nn.Conv2d(128, 128, 3, 1, 1)
        self.bn2 = torch.nn.BatchNorm2d(128)
        self.relu2 = torch.nn.ReLU()

    def forward(self, x):
        x_copy = x 
        x = self.relu1(self.bn1(self.conv1(x)))
        x = self.bn2(self.conv2(x))
        x = self.relu2(x + x_copy)
        return x

class PolicyHead(torch.nn.Module):
    def __init__(self):
        super(PolicyHead, self).__init__()
        self.conv = torch.nn.Conv2d(128, 32, 3, 1, 1)
        self.bn = torch.nn.BatchNorm2d(32)
        self.relu = torch.nn.ReLU()
        self.fc = torch.nn.Linear(32 * 9 * 10, 2086)

    def forward(self, x):
        x = self.relu(self.bn(self.conv(x)))
        x = x.view(-1, 32 * 9 * 10)
        x = self.fc(x)
        return x

class ValueHead(torch.nn.Module):
    def __init__(self):
        super(ValueHead, self).__init__()
        self.conv = torch.nn.Conv2d(128, 32, 3, 1, 1)
        self.bn = torch.nn.BatchNorm2d(32)
        self.relu = torch.nn.ReLU()
        self.fc = torch.nn.Linear(32 * 9 * 10, 128)
        self.relu2 = torch.nn.ReLU()
        self.fc2 = torch.nn.Linear(128, 1)
        self.tanh = torch.nn.Tanh()


    def forward(self, x):
        x = self.relu(self.bn(self.conv(x)))
        x = self.relu2(self.fc(x.view(-1, 32 * 9 * 10)))
        x = self.tanh(self.fc2(x))
        return x

class Model(torch.nn.Module):
    def __init__(self):
        super(Model, self).__init__()
        self.conv = torch.nn.Conv2d(15, 128, 3, 1, 1)
        self.relu = torch.nn.ReLU()
        self.resmods = torch.nn.Sequential(*[ResModule() for i in range(7)])

        self.policy = PolicyHead()
        self.value = ValueHead()

    def forward(self, x):
        x = self.relu(self.conv(x))

        x = self.resmods(x)

        pol = self.policy(x)
        val = self.value(x)

        return pol, val

    def param_num(self):
        total_num = sum(p.numel() for p in self.parameters())
        trainable_num = sum(p.numel() for p in self.parameters() if p.requires_grad)
        return {'Total': total_num, 'Trainable': trainable_num}

def test():
    x = torch.zeros((1, 15, 9, 10), dtype=torch.float32)
    net = Model()
    print(net)
    print(net.param_num())

    start = time.time()
    for i in range(100):
        p, v = net(x)
    end = time.time()
    elapsed = (end - start) * 1000
    print("elapsed: ", elapsed)
    print(p.size(), v.size(), v)

test()
