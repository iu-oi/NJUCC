import re


class Address:

    def __init__(self, seg, off):
        self.seg = seg
        self.off = off

    def set_value(self, value):
        self.seg[self.off] = value

    def get_value(self):
        return self.seg[self.off]

    def __str__(self):
        return self.name + ": " + self.get_value()

    def __add__(self, imm):
        return Address(self.seg, self.off + imm)

    def __sub__(self, imm):
        return Address(self.seg, self.off - imm)


class Code:

    def __init__(self, args):
        pass

    def __str__(self):
        return 'pad'


class CodeFunc(Code):
    pass


class CodeGlob(Code):
    pass


class CodeParam(Code):
    pass


class CodeArg(Code):
    pass


class CodeRet(Code):
    pass


class CodeRead(Code):
    pass


class CodeWrite(Code):
    pass


class CodeGoto(Code):
    pass


class CodeCall(Code):
    pass


class CodeMov(Code):
    pass


class CodeLoadAddr(Code):
    pass


class CodeLoad(Code):
    pass


class CodeStore(Code):
    pass


class CodeLocal(Code):
    pass


class CodeAdd(Code):
    pass


class CodeSub(Code):
    pass


class CodeMul(Code):
    pass


class CodeDiv(Code):
    pass


class CodeEq(Code):
    pass


class CodeNeq(Code):
    pass


class CodeGe(Code):
    pass


class CodeLe(Code):
    pass


class CodeGtr(Code):
    pass


class CodeLess(Code):
    pass


class Parser:

    __ops__ = {
        r'func (\w+):': CodeFunc,
        r'glob (\w+) (\w+)': CodeGlob,
        r'\((\w+)\) param (\w+)': CodeParam,
        r'\((\w+)\) arg (\w+)': CodeArg,
        r'\((\w+)\) ret (\w+)': CodeRet,
        r'\((\w+)\) read (\w+)': CodeRead,
        r'\((\w+)\) write (\w+)': CodeWrite,
        r'\((\w+)\) goto \((\w+)\)': CodeGoto,
        r'\((\w+)\) (\w+) = call (\w+)': CodeCall,
        r'\((\w+)\) (\w+) = (\w+)': CodeMov,
        r'\((\w+)\) (\w+) = &\w+': CodeLoadAddr,
        r'\((\w+)\) (\w+) = \*\w+': CodeLoad,
        r'\((\w+)\) \*\w+ = (\w+)': CodeStore,
        r'\((\w+)\) local (\w+) (\w+)': CodeLocal,
        r'\((\w+)\) (\w+) = (\w+) \+ (\w+)': CodeAdd,
        r'\((\w+)\) (\w+) = (\w+) - (\w+)': CodeSub,
        r'\((\w+)\) (\w+) = (\w+) \* (\w+)': CodeMul,
        r'\((\w+)\) (\w+) = (\w+) / (\w+)': CodeDiv,
        r'\((\w+)\) if (\w+) == (\w+) goto \((\w+)\)': CodeEq,
        r'\((\w+)\) if (\w+) != (\w+) goto \((\w+)\)': CodeNeq,
        r'\((\w+)\) if (\w+) >= (\w+) goto \((\w+)\)': CodeGe,
        r'\((\w+)\) if (\w+) <= (\w+) goto \((\w+)\)': CodeLe,
        r'\((\w+)\) if (\w+) > (\w+) goto \((\w+)\)': CodeGtr,
        r'\((\w+)\) if (\w+) < (\w+) goto \((\w+)\)': CodeLess
    }

    def parse(self, line):
        for pattern in self.__ops__.keys():
            matched = re.match(pattern, line)
            if matched is not None:
                code = object.__new__(self.__ops__[pattern])
                code.__init__(matched.groups())
                return code

    def __init__(self, machine, src):
        self.machine = machine
        self.src = src
        pass


class Machine:

    def __init__(self):
        self.text = [Code()]
        self.stack = []
        self.data = []
        self.names = {}

    def run(self, src):
        pass


def main():
    pass


if __name__ == '__main__':
    main()
